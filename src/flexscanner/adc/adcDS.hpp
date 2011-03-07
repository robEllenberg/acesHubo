#ifndef __ADCPACKET_HPP_
#define __ADCPACKET_HPP_

#include <iostream>

class adcPacket {
    public:
        adcPacket(unsigned char channel, unsigned char high,
                  unsigned char low);
        adcPacket(unsigned char channel, int val);
        void printme();
        unsigned char getChannel();
        int  getValue();
    private:
        unsigned char channel;
        int value;
};

#endif
