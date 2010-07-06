#include "svalue.hpp"

namespace ACES {

    SValue::SValue(float v=0.0){
        val = v;
    }

    void SValue::printme(){
        RTT::Logger::log() << this->val;
    }

    charDevSValue::charDevSValue(unsigned char c=0){
        cval = c;
    }

    void charDevSValue::printme(){
        RTT::Logger::log() << this->cval;
    }
}

