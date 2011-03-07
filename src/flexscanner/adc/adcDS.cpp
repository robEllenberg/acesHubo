#include "adcDS.hpp"

adcPacket::adcPacket(unsigned char ch, unsigned char high, unsigned char low){
    value = 0;
    value = high;
    value <<= 8;
    value |= low;
    channel = ch;
}

adcPacket::adcPacket(unsigned char ch, int val){
    value = val;
    channel = ch;
}

void adcPacket::printme(){
    std::cout << (int)channel << ": " << value << std::endl;
}

unsigned char adcPacket::getChannel(){
    return channel;
}

int  adcPacket::getValue(){
    return value;
}
