#include "protocol.hpp"

namespace ACES {
    ProtoProtocol::ProtoProtocol(std::string cfg, std::string args) :
      taskCfg(cfg),
      RTT::TaskContext(name)
    {}

    bool ProtoProtocol::subscribeDevice(RTT::TaskContext* d){
        this->connectPeers( (RTT::TaskContext*) d );

        RTT::base::PortInterface *myPort = NULL, *theirPort=NULL;
        bool success;
        RTT::ConnPolicy policy = RTT::ConnPolicy::buffer(10);
        
        theirPort = (RTT::base::PortInterface*)d->ports()->getPort("RxUS");
        myPort = (RTT::base::PortInterface*)this->ports()->getPort("TxUS");
        success = myPort->connectTo(theirPort, policy);
        if(not success){
            return false;
        }

        theirPort = (RTT::base::PortInterface*)d->ports()->getPort("TxDS");
        myPort = (RTT::base::PortInterface*)this->ports()->getPort("RxDS");
        success = theirPort->connectTo(myPort, policy);
        if(not success){
            return false;
        }

        /*RTT::Handle h = d->events()->setupConnection("txDownStream")
            .callback( this, &Protocol<HW,PD>::rxDownStream,
                       d->engine()->events()
                     ).handle();*/
        
        /*h = this->events()->setupConnection("txUpStream")
            .callback( d, &Device<S, PD>::rxUpStream
        //               ,this->engine()->events()
                     ).handle();*/
 
        return true;
    }
}

