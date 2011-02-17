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

%{
#include <iostream>
#include "adcDS.hpp"

//must be done for each distinct type of lexer made with flex++
//#undef yyFlexLexer
//#define yyFlexLexer adcFlexLexer
//#include <FlexLexer.h>

class adcFlex : adcFlexLexer
{
    public:
        adcFlex();
        adcPacket* releasePacket();
        int yylex();    //Flex provides an implemattion, but we must give the
                        //declaration
    private:
        int counter;
        adcPacket* packet;
        unsigned char tempByte;
};

%}

%x head
%x body
%option c++
%option noyywrap
%option prefix="adc"
%option yyclass="adcFlex"

%%
\xFF        {BEGIN(head);
             #ifndef NDEBUG 
             std::cout << "started head" << std::endl;
             #endif
             return 0;
            }

<head>[\x00-\xFE]        { BEGIN(INITIAL);
                           return 0;}

<head>\xFF	    { 
                  #ifndef NDEBUG 
                  std::cout << "Head found" << std::endl;
                  #endif
                  BEGIN(body);
                  return 0;}

<body>[\x00-\xFF]   {   if( (counter % 2) == 0){
                            counter++;
                            tempByte = yytext[0];
                            return 0;
                        }
                        else if( (counter % 2) == 1){
                            #ifndef NDEBUG  
                            std::cout << "Matched a packet." << std::endl;
                            #endif
                            packet = new adcPacket((counter+1)/2, tempByte, yytext[0]);
                            tempByte = 0;
                            if(counter == 5){
                                //If we're the third cycle, transition back to
                                //searching for a new header
                                counter = 0;
                                BEGIN(INITIAL);
                                #ifndef NDEBUG  
                                std::cout << "Matched a triplet." << std::endl;
                                #endif
                            }
                            else{
                                counter++;
                            }
                            return 1;
                            //Outer handler strips off packet, transmits it
                            //This implicitly zeros the packet counter
                        }
                    }

%%

adcFlex::adcFlex() : yyFlexLexer(){
    counter = 0;
    packet = NULL;
    tempByte = 0;
}

adcPacket* adcFlex::releasePacket(){
    adcPacket* p = packet;
    packet = NULL;
    return p;
}

#ifdef __ADC_PARSE_TESTSUITE__
    //#undef yyFlexLexer
    //#define yyFlexLexer adcFlexLexer
    //#include <FlexLexer.h>

    int main()
    {
        adcFlex adcflex;
        
        while ( 1 ){
            if(adcflex.yylex()){
                adcPacket *p = adcflex.releasePacket();
                p->printme();
                delete p;
            }
        }
    }
#endif