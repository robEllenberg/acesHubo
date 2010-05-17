#include "pvalue.hpp"

namespace ACES {

    PValue::PValue(float v=0.0){
        val = v;
    }

    void PValue::printme(){
        RTT::Logger::log() << this->val;
    }

    charDevPValue::charDevPValue(unsigned char c=0){
        cval = c;
    }

    void charDevPValue::printme(){
        RTT::Logger::log() << this->cval;
    }
}

