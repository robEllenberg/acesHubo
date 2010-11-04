#include "protocol.hpp"

namespace ACES {
    template <class HW, class P>    
    Protocol<HW,P>::Protocol(std::string cfg, std::string args):
      taskCfg(cfg),
      RTT::TaskContext(name),
      txDownStream("txDownStream"),
      txUpStream("txUpStream"),
      dsQueue(10),
      usQueue(10)
    {
        //requestBuf = new std::deque<Message*>;

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

    template <class HW, class P>    
    void Protocol<HW,P>::rxDownStream(PDWord<P> g){
        dsQueue.enqueue(g);
        //RTT::OS::MutexLock lock(dsqGuard);
        //dsQueue.push_back(g);
    }

    template <class HW, class P>    
    void Protocol<HW,P>::rxUpStream(HWord<HW> w){
        usQueue.enqueue(w);
        //RTT::OS::MutexLock lock(usqGuard);
        //usQueue.push_back(w);
    }
/*
    template <class T>
    Goal* Protocol<T>::getDSQelement(){
        RTT::OS::MutexLock lock(dsqGuard);
        Goal* g = dsQueue.front();
        dsQueue.pop_front();
        return g;
    }

    template <class T>
    Word<T> Protocol<T>::getUSQelement(){
        RTT::OS::MutexLock lock(usqGuard);
        Word<T> p = usQueue.front();
        usQueue.pop_front();
        return p;
    }
*/

    template <class HW, class P>    
        template<class S>
    bool Protocol<HW,P>::subscribeDevice(RTT::TaskContext* d){
        this->connectPeers( (RTT::TaskContext*) d );
        RTT::Handle h = d->events()->setupConnection("txDownStream")
            .callback( this, &Protocol<HW,P>::rxDownStream,
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
            .callback( d, &Device<S, P>::rxUpStream
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

    template <class HW, class P>
    bool Protocol<HW,P>::connectHardware(RTT::TaskContext* h){
        this->connectPeers(h);
        RTT::Handle hand = this->events()->setupConnection("txDownStream")
            .callback( h, &Hardware<HW>::rxDownStream
            //           ,p->engine()->events() ).handle();
            ).handle();
        if(! hand.ready() ){
            return false;
        }
        hand.connect();
        if(!hand.connected() ){
            return false;
        }

        hand = h->events()->setupConnection("txUpStream")
            .callback( this, &Protocol<HW,P>::rxUpStream
            //           ,this->engine()->events() ).handle();
            ).handle();

        if(! hand.ready() ){
            return false;
        }
        hand.connect();
        if(!hand.connected() ){
            return false;
        }
 
        return true;
    }

}

