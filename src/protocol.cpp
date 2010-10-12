#include "protocol.hpp"

namespace ACES {
    Protocol::Protocol(std::string cfg, std::string args):
      taskCfg(cfg),
      RTT::TaskContext(name),
      txDownStream("txDownStream"),
      txUpStream("txUpStream"),
      dsQueue(),
      usQueue()
    {
//        requestBuf = new std::deque<Message*>;

        //TODO - Figure out why this flips out when we attempt to declare
        // it as a member instead of a pointer 
        //returnBuf = new RTT::Buffer<ProtoResult*>(250);
        //returnQueue = new std::deque<ProtoResult*>(250);
        //requestQueue = new std::deque<Goal*>(250);
        
        this->events()->addEvent(&txDownStream, "txDownStream", "msg",
                                 "The message to be transmitted");
        this->events()->addEvent(&txUpStream, "txUpStream", "result",
                                 "Data struct containing processed result");
        this->setActivity(
            new RTT::Activity( priority, 1.0/freq, 0, name )
        );
    }

    void Protocol::rxDownStream(Goal* g){
        RTT::OS::MutexLock lock(dsqGuard);
        dsQueue.push_back(g);
    }

    void Protocol::rxUpStream(ProtoWord* w){
        RTT::OS::MutexLock lock(usqGuard);
        usQueue.push_back(w);
    }

    void Protocol::updateHook(){
        Message* m = NULL;
        assert(dsQueue.size() < 100);
        while(dsQueue.size()){
            m = processDSQueue();
            if(m){
                RTT::Logger::log() << RTT::Logger::Debug
                                   << "(Protocol) got DS" << RTT::endlog();
                //m->printme();
            }
            txDownStream(m);
        }

        ProtoResult* r = NULL;
        assert( usQueue->size() < 100);
        while(usQueue.size()){
            r = processUSQueue();
            if(r){
                //r->printme();
                RTT::Logger::log() << RTT::Logger::Debug
                                   << "(Protocol) got US" << RTT::endlog();
                txUpStream(r);
            }
        }
    }

    Message* Protocol::processDSQueue(){
        RTT::OS::MutexLock lock(dsqGuard);
        Goal* g = dsQueue.front();
        dsQueue.pop_front();
        Message* m = new Message(g);
        return m;
    }

    ProtoResult* Protocol::processUSQueue(){
        ProtoWord* p = NULL;
        { RTT::OS::MutexLock lock(usqGuard);
          p = usQueue.front();
          usQueue.pop_front();
        }
        Word<Goal*>* w = (Word<Goal*>*)p;
        Goal* g = w->data;
        int nID = g->nodeID;
        Result<Goal*>* r = new Result<Goal*>(g, g->cred, nID);
        return (ProtoResult*)r;
    }
    
    bool Protocol::subscribeDevice(Device* d){
        this->connectPeers( (RTT::TaskContext*) d );
        RTT::Handle h = d->events()->setupConnection("txDownStream")
            .callback( this, &Protocol::rxDownStream,
                       d->engine()->events()
                     ).handle();
        if(!h.ready()){
            return false;
        }
        h.connect();
        if(!h.connected() ){
            return false;
        }

        h = this->events()->setupConnection("txUpStream")
            .callback( d, &Device::rxUpStream
        //               ,this->engine()->events()
                     ).handle();
        if(!h.ready()){
            return false;
        }
        h.connect();
        if(!h.connected() ){
            return false;
        }
        return true;
    }
}

