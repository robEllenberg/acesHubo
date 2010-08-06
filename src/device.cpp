#include "device.hpp"

namespace ACES{
    Device::Device(std::string config, std::string junk) :
      taskCfg(config),
      RTT::TaskContext(name),
      txDownStream("txDownStream"),
      txUpStream("txUpStream"),
      dsQueue(),
      usQueue()
    {
        this->events()->addEvent(&txDownStream, "txDownStream", "goal",
                                 "The Goal/SP Data");
        this->events()->addEvent(&txUpStream, "txUpStream", "data",
                                 "Interperted data going to states");

        this->setActivity(
            //TODO - allow user to set priority
            new RTT::Activity( priority, 1.0/freq, 0, name )
        );
            
    }
/*
    Device::Device(std::string name) :
      RTT::TaskContext(name),
      txDownStream("txDownStream"),
      txUpStream("txUpStream")
    {
        this->events()->addEvent(&txDownStream, "txDownStream", "goal",
                                 "The Goal/SP Data");
        this->events()->addEvent(&txUpStream, "txUpStream", "data",
                                 "Interperted data going to states");

        dsQueue = new RTT::Buffer<Goal*>(50);
        usQueue = new RTT::Buffer<Goal*>(50);
    }
*/
    void Device::rxDownStream(Goal* g){
        //RTT::Logger::log() << "rxDS, device" << RTT::endlog();
        g->cred = credentials;
        RTT::OS::MutexLock lock(dsqGuard);
        dsQueue.push_back(g);
    }

    void Device::rxUpStream(ProtoResult* rx){
        //RTT::Logger::log() << "rxUS, device" << RTT::endlog();
        if(rx->devID == credentials->devID){
            rx->printme();
            RTT::OS::MutexLock lock(usqGuard);
            usQueue.push_back(rx);
        }
    }

/*
    void Device::attachCredentials(ACES::Credentials* c){
        credentials = c;
    }
*/
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
                     ,  this->engine()->events()
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
            //RTT::Logger::log() << "device sent" << RTT::endlog();
            txDownStream(g);
        }
        //Return Path
        assert(usQueue.size() < 100);
        while( usQueue.size() ){
            ProtoResult* p = processUSQueue();
            //RTT::Logger::log() << "device recieve" << RTT::endlog();
            //Credentials* c = p->cred;
            //p->printme();
            txUpStream(p);
        }
    }
    
    Goal* Device::processDSQueue(){
        RTT::OS::MutexLock lock(dsqGuard);
        Goal* g = dsQueue.front();
        dsQueue.pop_front();
        return g;
    }

    ProtoResult* Device::processUSQueue(){
        ProtoResult* p = NULL;
        { RTT::OS::MutexLock lock(usqGuard);
          p = usQueue.front();
          usQueue.pop_front();
        }
        Goal* g = ( (Result<Goal*>*)p)->result;
        Result<void*>* r = new Result<void*>(g->data, p->devID, p->nodeID);
        return (ProtoResult*)r;
    }
}
