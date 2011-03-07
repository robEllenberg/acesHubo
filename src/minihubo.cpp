#include "minihubo.hpp"

namespace MiniHubo{
    AdcHardware::AdcHardware(std::string cfg, std::string args)
      : FlexScanner::Hardware(cfg, args)
        {};

    AdcProtocol::AdcProtocol(std::string cfg, std::string args)
     : FlexScanner::Protocol<adcPacket, adcFlex>(cfg, args)
        {};

    ACES::Credentials* AdcProtocol::credFromPacket(adcPacket* p){
        return new ACES::Credentials(0);
    }

    ACES::Word<float>* AdcDevice::processUS(ACES::Word<adcPacket>* w){
        if(*(w->getCred()) == *credentials){
            int value = w->getData().getValue();

            float realVal = (float)value;

            ACES::Word<float> *res = new ACES::Word<float>(
                realVal /*data*/, w->getData().getChannel() /*nodeID*/,
                0 /*devID*/, ACES::REFRESH, credentials);
            res->printme();
            txUpStream.write(res);
        }
        return NULL;
    }
}
