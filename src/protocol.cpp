#include "protocol.hpp"

namespace ACES {
    Protocol::Protocol(std::string cfg, std::string args):
      taskCfg(cfg),
      RTT::TaskContext(name),
      txDownStream("txDownStream"),
      txUpStream("txUpStream"),
      dsQueue(200),
      usQueue(200)
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

/*
    Protocol::Protocol(std::string name)
      : RTT::TaskContext(name),
      txDownStream("txDownStream"),
      txUpStream("txUpStream")
    {
//        requestBuf = new std::deque<Message*>;

        //TODO - Figure out why this flips out when we attempt to declare
        // it as a member instead of a pointer 
        requestBuf = new RTT::Buffer<Message*>(250);
        returnBuf = new RTT::Buffer<ProtoResult*>(250);
        
        this->events()->addEvent(&txDownStream, "txDownStream", "msg",
                                 "The message to be transmitted");
        this->events()->addEvent(&txUpStream, "txUpStream", "result",
                                 "Data struct containing processed result");
    }
*/

    void Protocol::rxDownStream(Goal* g){
        RTT::OS::MutexLock lock(dsqGuard);
        dsQueue.push_back(g);
    }

    void Protocol::rxUpStream(ProtoWord* w){
        RTT::OS::MutexLock lock(usqGuard);
        usQueue.push_back(w);
    }

/*
    void Protocol::updateHook(){
       //Set/Request data path
        assert(dsQueue.size() < 100);
        while( dsQueue.size() ){
            //pop next message off pending  
            //m->printme();
            Message* m = NULL;
            {   //Block to delete mutexlok after pop()
                RTT::OS::MutexLock lock(pqGuard);
                m = new Message( requestQueue->front() );
                requestQueue->pop_front();
            }
            txDownStream( m );
        }

        //Return data path
        assert( usQueue->size() < 100);
        if(! returnBuf->empty() ){
            ProtoResult* r;
            returnBuf->Pop(r);
            txUpStream(r);
        }
    }
*/
    void Protocol::updateHook(){
        Message* m = NULL;
        assert(dsQueue.size() < 100);
        while(dsQueue.size()){
            { RTT::OS::MutexLock lock(dsqGuard);
              m = processDSQueue();
            }
            txDownStream(m);
        }

        ProtoResult* r = NULL;
        assert( usQueue->size() < 100);
        while(usQueue.size()){
            { RTT::OS::MutexLock lock(usqGuard);
              r = processUSQueue();
            }
            txUpStream(r);
        }
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
            .callback( d, &Device::rxUpStream,
                        this->engine()->events()
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

