#include "protocol.hpp"

namespace ACES {
    template <class T>
    void Protocol<T>::rxDownStream(Goal* g){
        RTT::OS::MutexLock lock(dsqGuard);
        dsQueue.push_back(g);
    }

    template <class T>
    void Protocol<T>::rxUpStream(Word<T> w){
        RTT::OS::MutexLock lock(usqGuard);
        usQueue.push_back(w);
    }

    template <class T>
    void Protocol<T>::updateHook(){
        Message<T>* m = NULL;
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

    template <class T>
    Goal* Protocol<T>::getDSQelement(){
        RTT::OS::MutexLock lock(dsqGuard);
        Goal* g = dsQueue.front();
        dsQueue.pop_front();
        return g;
    }

    template <class T>
    Message<T>* Protocol<T>::processDSQueue(){
        Goal* g = getDSQelement();
        Message<T>* m = new Message<T>(g);
        return m;
    }

    template <class T>
    Word<T> Protocol<T>::getUSQelement(){
        RTT::OS::MutexLock lock(usqGuard);
        Word<T> p = usQueue.front();
        usQueue.pop_front();
        return p;
    }

    template <class T>
    ProtoResult* Protocol<T>::processUSQueue(){
        Word<T> p = getUSQelement();
        Word<Goal*>* w = (Word<Goal*>*)p;
        Goal* g = w->data;
        int nID = g->nodeID;
        Result<Goal*>* r = new Result<Goal*>(g, g->cred, nID);
        return (ProtoResult*)r;
    }
    
    template <class T>
    bool Protocol<T>::subscribeDevice(Device* d){
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

