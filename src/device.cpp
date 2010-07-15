#include "device.hpp"

namespace ACES{
    Device::Device(taskCfg cfg, Credentials* c) :
      RTT::TaskContext(cfg.name),
      TxRequest("TxRequest")
    {
        credentials = c;
    }

    void Device::RxGoal(Goal* g){
        g->cred = credentials;
        TxRequest(g);
    }

    bool Device::subscribeState(RTT::TaskContext* s){
        RTT::Handle h = s->events()->setupConnection("announceGoal")
            .callback( this, &Device::RxGoal//,
                       //this->engine()->events()
                     ).handle();
        if( not h.ready() ){
            return false;
        }
        h.connect();
        if( not h.connected() ){
            return false;
        }
    }

}
