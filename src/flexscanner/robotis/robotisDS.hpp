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

#ifndef __ROBOTISPACKET_HPP_
#define __ROBOTISPACKET_HPP_

#include <iostream>
#include <deque>
#include <rtt/Logger.hpp>
#include "../../robotisTypes.hpp"

//must be done for each distinct type of lexer made with flex++
#ifndef yyFlexLexer
    #undef yyFlexLexer
    #define yyFlexLexer robotisFlexLexer
    #include <FlexLexer.h>
#endif

#ifndef yyFlexLexer
    #define yyFlexLexer robotisFlexLexer
#endif

namespace Robotis {
    class RobotisPacket {
        public:
            RobotisPacket();
            void printme();
            //~RobotisPacket();
            void setID(unsigned char id);
            void setLen(unsigned char l);
            void setError(unsigned char err);
            void setChecksum(unsigned char check);
            void setInst(INST inst);

            unsigned char getID();
            unsigned char getLen();
            unsigned char getError();
            unsigned char getChecksum();
            INST getInst();

            int counter;
            std::deque<unsigned char>* parameters;
        private:
            unsigned char id;
            unsigned char len;
            unsigned char error;
            unsigned char checksum;
            INST instruct;
    };
};

    class robotisFlex : public robotisFlexLexer
    {
        public:
            robotisFlex();
            Robotis::RobotisPacket* releasePacket();
            int yylex();    //Flex provides an implemattion,
                            //but we must give the declaration
            int yylex( std::istream* new_in, std::ostream* new_out = 0 );
        private:
            int counter;
            Robotis::RobotisPacket* packet;
            unsigned char tempByte;
    };
#endif
