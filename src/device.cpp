#include "device.hpp"

namespace ACES{
    Device::Device(std::string config) :
      taskCfg(config),
      RTT::TaskContext(name),
      txDownStream("txDownStream"),
      txUpStream("txUpStream"),
      dsQueue(10),
      usQueue(10),
      credMethod("credentials", &Device::printCred, this)
    {
        this->events()->addEvent(&txDownStream, "txDownStream", "goal",
                                 "The Goal/SP Data");
        this->events()->addEvent(&txUpStream, "txUpStream", "data",
                                 "Interperted data going to states");
        this->methods()->addMethod(credMethod, "credentials");

        this->setActivity(
            //TODO - allow user to set priority
            new RTT::Activity( priority, 1.0/freq, 0, name )
        );
            
    }

    void Device::rxDownStream(Goal* g){
        g->cred = credentials;
        RTT::OS::MutexLock lock(dsqGuard);
        dsQueue.push_back(g);
    }

    void Device::rxUpStream(ProtoResult* rx){
        //First we check if the two device types are the same
        if(*(rx->semiCred) == *credentials){
            RTT::Logger::log() << RTT::Logger::Debug
                               << "(dev) Cred Match" << RTT::endlog();
            RTT::OS::MutexLock lock(usqGuard);
            usQueue.push_back(rx);
        }
    }

    bool Device::subscribeState(ProtoState* s){
        this->connectPeers( (RTT::TaskContext*) s);
        //TODO - Figure out how calling this reception function
        //in a non-periodic thread affects RT characteristic
        RTT::Handle h = s->events()->setupConnection("txDownStream")
            .callback( this, &Device::rxDownStream
                     ,  s->engine()->events()
                     ).handle();
        if(!h.ready() ){
            return false;
        }
        h.connect();
        if(!h.connected() ){
            return false;
        }

        h = this->events()->setupConnection("txUpStream")
            .callback( s, &ProtoState::rxUpStream
          //           ,  this->engine()->events()
                     ).handle();
        if(!h.ready() ){
            return false;
        }
        h.connect();
        if(!h.connected() ){
            return false;
        }

        return true;
    }

    void Device::updateHook(){
        //Forward Path
        assert(dsQueue.size() < 100);
        Goal* g = NULL;
        while( dsQueue.size() ){
            g = processDSQueue();
            if(g){
                RTT::Logger::log() << RTT::Logger::Debug << "(dev) got DS"
                                   << RTT::endlog();
                txDownStream(g);
            }
        }
        //Return Path
        assert(usQueue.size() < 100);
        while( usQueue.size() ){
            std::list<ProtoResult*> p = processUSQueue();
            if(p.size()){
                RTT::Logger::log() << RTT::Logger::Debug << "(dev) got US"
                                   << RTT::endlog();
            }
            for(std::list<ProtoResult*>::iterator it = p.begin();
                it != p.end();  it++){
                    (*it)->printme();
                    txUpStream(*it);
                    RTT::Logger::log() << "(dev) nID:" << (*it)->nodeID
                                       << RTT::endlog();
            }
        }
    }
    
    Goal* Device::getDSQelement()
    {
        RTT::OS::MutexLock lock(dsqGuard);
        Goal* g = dsQueue.front();
        dsQueue.pop_front();
        return g;
    }

    Goal* Device::processDSQueue(){
        return getDSQelement(); 
    }

    ProtoResult* Device::getUSQelement()
    { 
        RTT::OS::MutexLock lock(usqGuard);
        ProtoResult* p = usQueue.front();
        usQueue.pop_front();
        return p;
    }

    std::list<ProtoResult*> Device::processUSQueue(){
        ProtoResult* p = getUSQelement();
        
        //RTT::Logger::log() << "(dev) got US" << RTT::endlog();
        Goal* g = ( (Result<Goal*>*) p)->result;
        Result<void*>* r = new Result<void*>(g->data, g->cred, p->nodeID);
        std::list<ProtoResult*> pr_list(1, (ProtoResult*)r);
        return pr_list;
    }

    bool Device::printCred(){
        credentials->printme();
    }
}
