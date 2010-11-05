#include "device.hpp"

namespace ACES{
    template <class S, class PD>
    Device<S,PD>::Device(std::string config) :
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

    template <class S, class PD>
    void Device<S,PD>::rxDownStream(Word<S>* g){
        bool i = g->setCred(credentials);
        assert(i);
        dsQueue.enqueue(g);
    }

    template <class S, class PD>
    void Device<S,PD>::rxUpStream(Word<PD>* rx){
        //First we check if the two device types are the same
        if(*(rx->getCred()) == *credentials){
            RTT::Logger::log() << RTT::Logger::Debug
                               << "(dev) Cred Match" << RTT::endlog();
            //If they are, we process them
            usQueue.enqueue(rx);
        }
    }

    template <class S, class PD>
    bool Device<S,PD>::subscribeState(RTT::TaskContext* s){
        this->connectPeers( (RTT::TaskContext*) s);
        //TODO - Figure out how calling this reception function
        //in a non-periodic thread affects RT characteristic
        RTT::Handle h = s->events()->setupConnection("txDownStream")
            .callback( this, &Device<S,PD>::rxDownStream
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
}
