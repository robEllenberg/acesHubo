#include "device.hpp"

namespace ACES{

    Device::Device(std::string config) :
      taskCfg(config),
      TxRequest("TxRequest"),
      RTT::TaskContext(name)
    {
        this->events()->addEvent(&TxRequest, "TxRequest", "goal",
                                 "The Goal/SP Data");
        this->setActivity(
            //TODO - allow user to set priority
            new RTT::Activity( 5, 0, 0, name )
        );
            
    }

    void Device::RxGoal(Goal* g){
        g->cred = credentials;
        TxRequest(g);
    }

    bool Device::subscribeState(ProtoState* s){
        this->connectPeers( (RTT::TaskContext*) s);
        RTT::Handle h = s->events()->setupConnection("announceGoal")
            .callback( this, &Device::RxGoal
                     ,  this->engine()->events()
                     ).handle();
        if( not h.ready() ){
            return false;
        }
        h.connect();
        if( not h.connected() ){
            return false;
        }
    }

    bool Device::configureHook(){
        return true;
    }

    bool Device::startHook(){
        return true;
    }

    void Device::updateHook(){}
    void Device::stopHook(){}
    void Device::cleanupHook(){}

}
