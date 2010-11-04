#include "device.hpp"

namespace ACES{
    template <class S, class P>
    Device<S,P>::Device(std::string config) :
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

    template <class S, class P>
    void Device<S,P>::rxDownStream(SWord<S>* g){
        g->cred = credentials;
        //RTT::OS::MutexLock lock(dsqGuard);
        dsQueue.enqueue(g);
        //dsQueue.push_back(g);
    }

    template <class S, class P>
    void Device<S,P>::rxUpStream(PDWord<P>* rx){
        //First we check if the two device types are the same
        if(*(rx->semiCred) == *credentials){
            RTT::Logger::log() << RTT::Logger::Debug
                               << "(dev) Cred Match" << RTT::endlog();
            //RTT::OS::MutexLock lock(usqGuard);
            //usQueue.push_back(rx);
            usQueue.enqueue(rx);
        }
    }

    template <class S, class P>
    bool Device<S,P>::subscribeState(RTT::TaskContext* s){
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
            .callback( s, &State<S>::rxUpStream
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

    template <class S, class P>
    void Device<S,P>::updateHook(){
        //Forward Path
        assert(dsQueue.size() < 100);
        SWord<S>* g = NULL;
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
            std::deque< PDWord<P>* >p;
            p = processUSQueue();
            if(p.size()){
                RTT::Logger::log() << RTT::Logger::Debug << "(dev) got US"
                                   << RTT::endlog();
            }
            typename std::deque< PDWord<P>* >::iterator it;
            for(it = p.begin();
                it != p.end();  it++){
                    (*it)->printme();
                    txUpStream(*it);
                    RTT::Logger::log() << "(dev) nID:" << (*it)->nodeID
                                       << RTT::endlog();
            }
        }
    }
    
/*
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
*/

    template <class S, class P>
    std::deque< PDWord<P>* > Device<S,P>::processUSQueue(){
        SWord<S>* s = NULL;
        usQueue.dequeue(s);
        PDWord<P>* p = new PDWord<P>(s.getData(), s.getNodeID(),
                                     s.getDevID(), s.getCred);
        
        return std::deque< PDWord<P>* >(1, p);
        //RTT::Logger::log() << "(dev) got US" << RTT::endlog();
        //Goal* g = ( (Result<Goal*>*) p)->result;
        //Result<void*>* r = new Result<void*>(g->data, g->cred, p->nodeID);
        //std::list<ProtoResult*> pr_list(1, (ProtoResult*)r);
        //return pr_list;
    }

    template <class S, class P>
    bool Device<S,P>::printCred(){
        credentials->printme();
    }

}
