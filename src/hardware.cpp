#include "hardware.hpp"
namespace ACES {
    ProtoHardware::ProtoHardware(std::string cfg, std::string args) :
      taskCfg(cfg),
      RTT::TaskContext(name)
    {
    }


    bool ProtoHardware::subscribeController(RTT::TaskContext* c){
        return true;
    }

    bool ProtoHardware::subscribeProtocol(RTT::TaskContext* p){
        this->connectPeers(p);
   //     RTT::Handle hand = this->events()->setupConnection("txDownStream")
   //         .callback( h, &Hardware<HW>::rxDownStream
            //           ,p->engine()->events() ).handle();
   //         ).handle(); 
        //RTT::base
        RTT::base::PortInterface *myPort = NULL, *theirPort=NULL;
        bool success;
        RTT::ConnPolicy policy = RTT::ConnPolicy::buffer(10);

        theirPort = (RTT::base::PortInterface*)p->ports()->getPort("RxUS");
        myPort = (RTT::base::PortInterface*)this->ports()->getPort("TxUS");
        success = myPort->connectTo(theirPort, policy);
        if(not success){
            return false;
        }
        
        theirPort = (RTT::base::PortInterface*)p->ports()->getPort("TxDS");
        myPort = (RTT::base::PortInterface*)this->ports()->getPort("RxDS");
        success = theirPort->connectTo(myPort, policy);
        if(not success){
            return false;
        }
                                  
      //  hand = h->events()->setupConnection("txUpStream")
      //      .callback( this, &Protocol<HW,PD>::rxUpStream
            //           ,this->engine()->events() ).handle();
      //      ).handle();
        return true;
    }
    
}
