#include "device.hpp"

namespace ACES{
    Device::Device(std::string config) :
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

    void Device::rxDownStream(Goal* g){
        g->cred = credentials;
        RTT::OS::MutexLock lock(dsqGuard);
        dsQueue.push_back(g);
    }

    void Device::rxUpStream(ProtoResult* rx){
        //First we check if the two device types are the same
        if(rx->semiCred == credentials){
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
            txDownStream(g);
        }
        //Return Path
        assert(usQueue.size() < 100);
        while( usQueue.size() ){
            ProtoResult* p = processUSQueue();
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
        Result<void*>* r = new Result<void*>(g->data, g->cred, p->nodeID);
        return (ProtoResult*)r;
    }
}
