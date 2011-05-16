#ifndef __ADCPACKET_HPP_
#define __ADCPACKET_HPP_

#include <iostream>
#include "adcDS.hpp"

//must be done for each distinct type of lexer made with flex++
#ifndef yyFlexLexer
    #undef yyFlexLexer
    #define yyFlexLexer adcFlexLexer
    #include <FlexLexer.h>
#endif

#ifndef yyFlexLexer
    #define yyFlexLexer adcFlexLexer
#endif

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

class adcFlex : public adcFlexLexer
{
    public:
        adcFlex();
        adcPacket* releasePacket();
        int yylex();    //Flex provides an implemattion, but we must give the
                        //declaration
        int yylex( std::istream* new_in, std::ostream* new_out = 0 );
    private:
        int counter;
        adcPacket* packet;
        unsigned char tempByte;
};

#endif
