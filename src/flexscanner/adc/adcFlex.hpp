/* AUTORIGHTS
    ACES: Abstracted Controls Engineering Software
    Copyright (C) 2010 Robert Sherbert
    bob.sherbert@gmail.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    If you are interested in licensing this software for commercial purposes
    please contact the author.
*/

#ifndef ACES_ADCFLEXSCAN_HPP
#define ACES_ADCFLEXSCAN_HPP

#include <iostream>
#include <istream>
#include <ostream>
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
