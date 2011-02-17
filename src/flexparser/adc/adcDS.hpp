#ifndef __ADCPACKET_HPP_
#define __ADCPACKET_HPP_

class adcPacket {
    public:
        adcPacket(char channel, int value);
        void printme();
        char getChannel();
        int  getValue();
    private:
        unsigned char channel;
        float value;
};

class adcFlex : adcFlexLexer
{
    private:
        int counter;
        int yylex();
        adcPacket* packet;
};

#endif
