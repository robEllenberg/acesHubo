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
#include "src/robotis.hpp"

%}

%x head
%x id_frame
%x len_frame
%x error_frame
%x param_frame
%x check_frame
%option reentrant
%option noyywrap

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

<id_frame>[\x00-\xFF]   {  MAKEP
                          p->setID(*yytext);
                          //std::cout << "got id" << std::endl;
                          BEGIN(len_frame);
                          return 0;}

<len_frame>[\x00-\xFF]    { MAKEP
                            p->setLen(*yytext);
                            p->counter = 0; //p->getLen() - 2; 
                            //std::cout << "got len" << std::endl;
                            BEGIN(error_frame);
                            return 0;}

<error_frame>[\x00-\xFF]    { MAKEP
                              p->setError(*yytext); 
                              p->counter += 1;
                              //std::cout << "got error" << std::endl;
                             if( (p->counter + 1) >= p->getLen() ){   //The length of n include the checksum
                              BEGIN(check_frame);
                             }
                             else{
                              BEGIN(param_frame);
                             }
                              return 0;}

<param_frame>[\x00-\xFF]     { MAKEP  
                                  //std::cout << "got parms" << std::endl;
                                  (*p).parameters->push_back(*yytext);
                                  (p->counter)++;
                                 if( (p->counter + 1) >= p->getLen() ){   //The length of n include the checksum
                                  BEGIN(check_frame); 
                                 }
                                 return 0;
                             }

<check_frame>[\x00-\xFF]       { MAKEP
                          p->setChecksum(*yytext);
                          //std::cout << "got check" << std::endl;
                          //(*p).printme();
                          yyset_extra( p, yyscanner );
                          BEGIN(INITIAL);
                          return 1;
                          } 
%%

/*
int main()
{
    yyscan_t scanner;
    yylex_init ( &scanner ) ;
    //yylex(scanner);
    int tok;
    packet* p = new packet;
    yyset_extra(p, scanner);
    while ((tok=yylex(scanner)) > 0){
        //std::cout << "tok=" << tok << "yytext=" << yyget_text(scanner) << std::endl;
        p = yyget_extra( scanner);
        std::cout << "id is " << (*p).id << std::endl;
    }
    
    yylex_destroy( scanner);
}
*/
