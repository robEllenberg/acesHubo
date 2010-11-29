#include "device.hpp"

namespace ACES{
    ProtoDevice::ProtoDevice(std::string config) :
      taskCfg(config),
      RTT::TaskContext(name),
      DSlockout(false),  USlockout(false)
        {}

    bool ProtoDevice::subscribeState(RTT::TaskContext* s){
        this->connectPeers( (RTT::TaskContext*) s);
        //TODO - Figure out how calling this reception function
        //in a non-periodic thread affects RT characteristic

        RTT::base::PortInterface *myPort = NULL, *theirPort=NULL;
        bool success;
        RTT::ConnPolicy policy = RTT::ConnPolicy::buffer(200);

        theirPort = (RTT::base::PortInterface*)s->ports()->getPort("RxUS");
        myPort = (RTT::base::PortInterface*)this->ports()->getPort("TxUS");
        success = myPort->connectTo(theirPort, policy);
        if(not success){
            return false;
        }
        /*RTT::Handle h = s->events()->setupConnection("txDownStream")
            .callback( this, &Device<S,PD>::rxDownStream
                     ,  s->engine()->events()
                     ).handle();*/

        theirPort = (RTT::base::PortInterface*)s->ports()->getPort("TxDS");
        myPort = (RTT::base::PortInterface*)this->ports()->getPort("RxDS");
        success = theirPort->connectTo(myPort, policy);
        if(not success){
            return false;
        }
        /*h = this->events()->setupConnection("txUpStream")
            .callback( s, &State<S>::rxUpStream
          //           ,  this->engine()->events()
                     ).handle();*/
        return true;
    }
/*
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
*/
}
