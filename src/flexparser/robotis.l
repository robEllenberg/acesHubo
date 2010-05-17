%{
#include <iostream>
#include "packet.hpp"

#define YY_EXTRA_TYPE packet*
typedef void* yyscan_t;

extern "C" {
    int yylex ( yyscan_t yyscanner );
    //int yylex();
    int yylex_init( yyscan_t * ptr_yy_globals );
    int yylex_destroy(yyscan_t yyscanner);
    YY_EXTRA_TYPE yyget_extra(yyscan_t yyscanner);
    void yyset_extra (YY_EXTRA_TYPE p, yyscan_t yyscanner);
}
packet* p ;
int n = 0; //A counter we can keep around
%}

%x id_frame
%x len_frame
%x error_frame
%x param_frame
%x check_frame
%option reentrant
%option noyywrap

%%
\xFF\xFF	    { std::cout << "Head found\n";
                  //p = new packet;
                  p = yyget_extra(yyscanner);
                  BEGIN(id_frame);
                  return 0;}

<id_frame>[\x00-\xFF]   { (*p).id = *yytext;
                          std::cout << "got id" << std::endl;
                          BEGIN(len_frame); 
                          return 0;}

<len_frame>[\x00-\xFF]    { (*p).len = *yytext;
                            n = (*p).len - 2; 
                            std::cout << "got len" << std::endl;
                            BEGIN(error_frame);
                            return 0;}

<error_frame>[\x00-\xFF]    { (*p).error = *yytext; 
                              std::cout << "got error" << std::endl;
                              BEGIN(param_frame);
                              return 0;}

<param_frame>[\x00-\xFF]     {   if(n-1 > 0){   //The length of n include the checksum
                                  (*p).parameters->push_back(*yytext);
                                  n--;
                                 } else{
                                  std::cout << "got parms" << std::endl;
                                  BEGIN(check_frame); 
                                 }
                                 return 0;
                             }

<check_frame>[\x00-\xFF]       { (*p).checksum = *yytext;
                          std::cout << "got check" << std::endl;
                          (*p).printme();
                          yyset_extra( p, yyscanner );
                          BEGIN(INITIAL);
                          return 1;
                          } 
%%

int main()
{
    yyscan_t scanner;
    yylex_init ( &scanner ) ;
    //yylex(scanner);
    int tok;
    packet* p = new packet;
    yyset_extra(p, scanner);
    while ( ! yylex(scanner) ){
    }
    p = yyget_extra( scanner);
    std::cout << "id is " << (int)(*p).id << std::endl;
    
    yylex_destroy( scanner);
}
