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
    enum COMP_TYPE { JOINT=3 };

    enum INST { PING=1, READ, WRITE, REG_WRITE, ACTION, RESET, SYNC_WRITE=0x83 };

    enum PARAM_TABLE { 
            MODEL_NUMBER = 0x0,                VERSION_OF_FIRMWARE = 0x2,
            ID = 0x3,                          BAUD_RATE = 0x4,
            RETURN_DELAY_TIME = 0x5,           CW_ANGLE_LIMIT = 0x6,
            CCW_ANGLE_LIMIT = 0x8,             HIGHEST_LIMIT_TEMPERATURE = 0xB,
            LOWEST_LIMIT_VOLTAGE = 0xC,        HIGHEST_LIMIT_VOLTAGE = 0xD,
            MAX_TORQUE = 0xE,                  STATUS_RETURN_LEVEL = 0x10,
            ALARM_LED = 0x11,                  ALARM_SHUTDOWN = 0x12,

            TORQUE_ENABLE = 0x18,              LED = 0x19,
            CW_COMPLIANCE_MARGIN = 0x1A,       CCW_COMPLIANCE_MARGIN = 0x1B,
            CW_COMPLIANCE_SCOPE = 0x1C,        CCW_COMPLIANCE_SCOPE = 0x1D,
            GOAL_POSITION = 0x1E,              MOVING_SPEED = 0x20,
            TORQUE_LIMIT = 0x22,               PRESENT_POSITION = 0x24,
            PRESENT_SPEED = 0x26,              PRESENT_LOAD = 0x28,
            PRESENT_VOLTAGE = 0x2A,            PRESENT_TEMPERATURE = 0x2B,
            REGISTERED_INSTRUCTION = 0x2C,     MOVING = 0x2E,
            LOCK = 0x2F,                       PUNCH = 0x30
    };

    const unsigned char PARAM_LEN[] = {
            2, 0,           //MODEL_NUMBER = 0x0
            1,              //VERSION_OF_FIRMWARE = 0x2
            1,              //ID = 0x3
            1,              //BAUD_RATE = 0x4
            1,              //RETURN_DELAY_TIME = 0x5
            2, 0,           //CW_ANGLE_LIMIT = 0x6
            2, 0,           //CCW_ANGLE_LIMIT = 0x8
            0,
            1,              //HIGHEST_LIMIT_TEMPERATURE = 0xB
            1,              //LOWEST_LIMIT_VOLTAGE = 0xC
            1,              //HIGHEST_LIMIT_VOLTAGE = 0xD
            2,              //MAX_TORQUE = 0xE
            1,              //STATUS_RETURN_LEVEL = 0x10
            1,              //ALARM_LED = 0x11
            1,              //ALARM_SHUTDOWN = 0x12
            0, 0, 0, 0, 0,
            1,              //TORQUE_ENABLE = 0x18
            1,              //LED = 0x19
            1,              //CW_COMPLIANCE_MARGIN = 0x1A
            1,              //CCW_COMPLIANCE_MARGIN = 0x1B
            1,              //CW_COMPLIANCE_SCOPE = 0x1C
            1,              //CCW_COMPLIANCE_SCOPE = 0x1D
            2, 0,           //GOAL_POSITION = 0x1E
            2, 0,           //MOVING_SPEED = 0x20
            2, 0,           //TORQUE_LIMIT = 0x22
            2, 0,           //PRESENT_POSITION = 0x24
            2, 0,           //PRESENT_SPEED = 0x26
            2, 0,           //PRESENT_LOAD = 0x28
            1,              //PRESENT_VOLTAGE = 0x2A
            1,              //PRESENT_TEMPERATURE = 0x2B
            1,              //REGISTERED_INSTRUCTION = 0x2C
            0, 0,
            1,              //MOVING = 0x2E
            1,              //LOCK = 0x2F
            2, 0            //PUNCH = 0x30
    };

    class RobotisPacket {
        public:
            RobotisPacket();
            ~RobotisPacket();
            int counter;
            unsigned char id;
            unsigned char len;
            unsigned char error;
            unsigned char instruct;
            std::vector<unsigned char>* parameters;
            unsigned char checksum;
            void printme();
    };

    class Protocol : public ACES::Protocol {
        public:
            Protocol(std::string cfg, std::string args); 
            bool startHook();
            void stopHook();
            ACES::ProtoResult* processUSQueue();
            ACES::Message* processDSQueue();

            //~Protocol();
            //ACES::Message* buildMessage(ACES::Credentials* cred);

            yyscan_t scanner;
            RobotisPacket* curPacket;
            unsigned char incoming_id;
            unsigned char incoming_len;
    };

    class Device : public ACES::Device {
        public:
            Device(std::string config, std::string args);
            bool startHook();
            void stopHook();
            virtual ACES::Goal* Device::processDSQueue();
            virtual std::list<ACES::ProtoResult*> processUSQueue();
        private:
            int requestPos; //!The memory table position of the last request issued
            int requestLen; //!The size of data from the last issued request
            bool lockout;
    };

    class Credentials : public ACES::Credentials {
        public:
            Credentials(int motNum, float zero, float dir);
            static Credentials* makeCredentials(std::string args);
            virtual void printme();
            virtual bool operator==(ACES::Credentials& other);

            float zero;
            float direction;
            int motorNum;
    };
    
    Credentials* credFromPacket(RobotisPacket* p);
    unsigned char checksum(std::string& str);
    float USScale(int in, int nodeID);
    unsigned short DSScale(float in, int nodeID);
    float USlimit(float c, float low, float high);
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
