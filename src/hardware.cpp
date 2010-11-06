#include "hardware.hpp"
namespace ACES {
    ProtoHardware::ProtoHardware(){
    }

    bool ProtoHardware::rxBus(){
        return true;
    }

    bool ProtoHardware::subscribeController(Controller* c){
        return true;
    }

}
