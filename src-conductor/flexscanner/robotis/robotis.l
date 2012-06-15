/* 
    Conductor: High Degree of Freedom Robot Controller Framework
    Copyright (C) 2010, 2011 Robert Sherbert
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
    please contact the author. The software can be licensed to you under
    non-free license terms by the copyright holder.

    As a courtesy to the author, and in the spirit of fair attribution, you are
    asked to cite the following paper if any work based upon or utilizing this
    framework is published in the scientific literature: 
    Sherbert, Robert M. and Oh, Paul Y. "Conductor: A Controller Development
    Framework for High Degree of Freedom Systems." Intelligent Robots and
    Systems (IROS), 2011 IEEE/RSJ International Conference on. 
*/

%{
#include "robotisDS.hpp"
%}

%x head
%x id_frame
%x len_frame
%x error_frame
%x param_frame
%x check_frame
%option c++
%option noyywrap
%option prefix="robotis"
%option yyclass="robotisFlex"

%%
\xFF        {BEGIN(head);
             //std::cout << "started head\n";
             return 0;}

<head>[\x00-\xFE]        { BEGIN(INITIAL);
                           return 0;}

<head>\xFF	    { //std::cout << "Head found\n";
                  //MAKEP
                  BEGIN(id_frame);
                  return 0;}

<id_frame>[\x00-\xFF]   { packet->setID(yytext[0]); 
                          //std::cout << "got id" << std::endl;
                          BEGIN(len_frame);
                          return 0;}

<len_frame>[\x00-\xFF]    { packet->setLen(yytext[0]);
                            packet->counter = 0; //p->getLen() - 2; 
                            //std::cout << "got len" << std::endl;
                            BEGIN(error_frame);
                            return 0;}

<error_frame>[\x00-\xFF]    { packet->setError(yytext[0]); 
                              packet->counter += 1;
                              //std::cout << "got error" << std::endl;
                             if( (packet->counter + 1) >= packet->getLen() ){
                               //The length of n include the checksum
                              BEGIN(check_frame);
                             }
                             else{
                              BEGIN(param_frame);
                             }
                              return 0;}

<param_frame>[\x00-\xFF]     { //std::cout << "got parms" << std::endl;
                               packet->parameters->push_back(yytext[0]);
                               packet->counter++;
                               if( (packet->counter + 1) >= packet->getLen() ){
                                 //The length of n include the checksum
                                  BEGIN(check_frame); 
                                 }
                                 return 0;
                             }

<check_frame>[\x00-\xFF]       { packet->setChecksum(yytext[0]);
                          //std::cout << "got check" << std::endl;
                          //(*p).printme();
                          //yyset_extra( packet, yyscanner );
                          BEGIN(INITIAL);
                          return 1;
                          } 
%%

    robotisFlex::robotisFlex() : yyFlexLexer(){
        counter = 0;
        packet = NULL;
        tempByte = 0;
    }

    Robotis::RobotisPacket* robotisFlex::releasePacket(){
        Robotis::RobotisPacket* p = packet;
        packet = NULL;
        return p;
    }

    int robotisFlex::yylex( std::istream* new_in, std::ostream* new_out ){
        switch_streams(new_in, new_out );
        yylex(new_in, new_out);
        return 0;
    }

#ifdef __ROBOTIS_PARSE_TESTSUITE__
    int main()
    {
        robotisFlex robotisflex;
        
        while ( 1 ){
            if(robotisflex.yylex()){
                RobotisPacket *p = robotisflex.releasePacket();
                p->printme();
                delete p;
            }
        }
    }
#endif
