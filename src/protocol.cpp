#include "protocol.hpp"

namespace ACES {
    template <class HW, class PD>    
    Protocol<HW,PD>::Protocol(std::string cfg, std::string args):
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

    template <class HW, class PD>    
    void Protocol<HW,PD>::rxDownStream(Word<PD>* g){
        dsQueue.enqueue(g);
    }

    template <class HW, class PD>    
    void Protocol<HW,PD>::rxUpStream(Word<HW>* w){
        usQueue.enqueue(w);
    }

    template <class HW, class PD>    
        template<class S>
    bool Protocol<HW,PD>::subscribeDevice(RTT::TaskContext* d){
        this->connectPeers( (RTT::TaskContext*) d );
        RTT::Handle h = d->events()->setupConnection("txDownStream")
            .callback( this, &Protocol<HW,PD>::rxDownStream,
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
            .callback( d, &Device<S, PD>::rxUpStream
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

    template <class HW, class PD>
    bool Protocol<HW,PD>::connectHardware(RTT::TaskContext* h){
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
            .callback( this, &Protocol<HW,PD>::rxUpStream
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

