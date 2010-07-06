#ifndef ACES_ROBOTIS_HPP
#define ACES_ROBOTIS_HPP

#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstring>

#include <rtt/TaskContext.hpp>
#include <rtt/Logger.hpp>

#include "protocol.hpp"
#include "message.hpp"
#include "state.hpp"
//#include "dispatcher.hpp"

//Defs for the lexer
//#define YY_BUFFER_STATE yy_buffer_state*
//TODO - find out the actual return type of YY_BUFFER_STATE (hint: it's not char*)
extern "C"{

    #define YY_EXTRA_TYPE Robotis::RobotisPacket*
    typedef struct yy_buffer_state *YY_BUFFER_STATE;
    typedef size_t yy_size_t;
    #define MAKEP Robotis::RobotisPacket *p = yyget_extra(yyscanner);

    struct yy_buffer_state;
    typedef void* yyscan_t;

}

namespace Robotis {

    class RobotisPacket {
        public:
            RobotisPacket();
            ~RobotisPacket();
            int counter;
            unsigned char id;
            unsigned char len;
            unsigned char error;
            std::vector<unsigned char>* parameters;
            unsigned char checksum;
            void printme();
    };

    unsigned char checksum(std::string& str);
    float angleScale(unsigned char low, unsigned char high);

    class Protocol : public ACES::Protocol {
        public:
            yyscan_t scanner;
            RobotisPacket* curPacket;
            unsigned char incoming_id;
            unsigned char incoming_len;

            Protocol(std::string n, ACES::Hardware* hw,
                     int pri, int UpdateFreq);

            ~Protocol();

            std::vector<ACES::Message*>* interpreter(
                     std::deque<char>*);

            std::deque<char>* scanInput();

            void addRequest(ACES::Credentials* c);

            ACES::Message* buildMessage(
                 ACES::Credentials* cred);

            ACES::Credentials* parseHWInput(unsigned char* c);
    };
    
    class State : public ACES::State {
        public:
            State(std::string n,
                ACES::Credentials* c, ACES::Dispatcher* d,
                int pri, int UpdateFreq);
    };
    
    class Credentials : public ACES::Credentials {
        public:
            Credentials(int id_num, float ang);
            Credentials(int id_num);
            Credentials();
            int id;
            float angle;
    };
    
    class Hardware : public ACES::charDevHardware {
        public:
            Hardware(std::string name,
                std::ifstream *in, std::ofstream *out,
                int priority, int UpdateFreq);
            //ACES::Hardware* operator<<(ACES::Message* m);
    };
    
}    

//Function defs for the lexer
extern "C" {
    int yylex ( yyscan_t yyscanner );
    //int yylex();
    int yylex_init( yyscan_t * ptr_yy_globals );
    int yylex_destroy(yyscan_t yyscanner);
    YY_EXTRA_TYPE yyget_extra(yyscan_t yyscanner);
    void yyset_extra (YY_EXTRA_TYPE p, yyscan_t yyscanner);
};
    YY_BUFFER_STATE yy_scan_bytes(const char* bytes, int len, yyscan_t yyscanner);

#endif
