%{
#include <iostream>
#include "robotis.hpp"

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
                          (*p).id = *yytext;
                          //std::cout << "got id" << std::endl;
                          BEGIN(len_frame);
                          return 0;}

<len_frame>[\x00-\xFF]    { MAKEP
                            (*p).len = *yytext;
                            ((*p).counter) = (*p).len - 2; 
                            //std::cout << "got len" << std::endl;
                            BEGIN(error_frame);
                            return 0;}

<error_frame>[\x00-\xFF]    { MAKEP
                              (*p).error = *yytext; 
                              //std::cout << "got error" << std::endl;
                              BEGIN(param_frame);
                              return 0;}

<param_frame>[\x00-\xFF]     { MAKEP  
                                 if( ((*p).counter) - 1 > 0){   //The length of n include the checksum
                                  (*p).parameters->push_back(*yytext);
                                  ((*p).counter)--;
                                 } else{
                                  //std::cout << "got parms" << std::endl;
                                  BEGIN(check_frame); 
                                 }
                                 return 0;
                             }

<check_frame>[\x00-\xFF]       { MAKEP
                          (*p).checksum = *yytext;
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
