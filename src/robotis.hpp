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

#ifndef ACES_ROBOTIS_HPP
#define ACES_ROBOTIS_HPP

#include <deque>
#include <string>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <sstream>

#include <rtt/TaskContext.hpp>
#include <rtt/Logger.hpp>
#include <rtt/base/RunnableInterface.hpp>
#include <rtt/Activity.hpp>

#include <boost/asio.hpp>

#include "protocol.hpp"
#include "message.hpp"
#include "state/state.hpp"
#include "hardware.hpp"
#include "credentials.hpp"
#include "flexscanner/robotis/robotisDS.hpp"
#include "flexscanner/flexscanner.hpp"
#include "robotisTypes.hpp"

//Defs for the lexer
//#define YY_BUFFER_STATE yy_buffer_state*
//TODO - find out the actual return type of YY_BUFFER_STATE (hint: it's not char*)
/*
extern "C"{
    #define YY_EXTRA_TYPE Robotis::RobotisPacket*
    typedef struct yy_buffer_state *YY_BUFFER_STATE;
    typedef size_t yy_size_t;
    #define MAKEP Robotis::RobotisPacket *p = yyget_extra(yyscanner);

    struct yy_buffer_state;
    typedef void* yyscan_t;
}
*/
namespace Robotis {
    const unsigned char PARAM_LEN[] = {
            2, 0,           //MODEL_NUMBER = 0x0
            1,              //VERSION_OF_FIRMWARE = 0x2
            1,              //ID = 0x3
            1,              //BAUD_RATE = 0x4
            1,              //RETURN_DELAY_TIME = 0x5
            2, 0,           //CW_ANGLE_LIMIT = 0x6
            2, 0,           //CCW_ANGLE_LIMIT = 0x8
            1,              //Deadspace = 0xA
            1,              //HIGHEST_LIMIT_TEMPERATURE = 0xB
            1,              //LOWEST_LIMIT_VOLTAGE = 0xC
            1,              //HIGHEST_LIMIT_VOLTAGE = 0xD
            2, 0,           //MAX_TORQUE = 0xE
            1,              //STATUS_RETURN_LEVEL = 0x10
            1,              //ALARM_LED = 0x11
            1,              //ALARM_SHUTDOWN = 0x12
            1, 1, 1, 1, 1,  //Deadspace
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
            1,              //Deadspace
            1,              //MOVING = 0x2E
            1,              //LOCK = 0x2F
            2, 0            //PUNCH = 0x30
    };


    class RobotisQueue {
        public:
            bool addPacket(RobotisPacket p);
            std::vector<ACES::Message<unsigned char>*> renderAllMessages();
            //ACES::Message<unsigned char>*
            //  renderMessageFromQueue(PARAM_TABLE param);
            ACES::Message<unsigned char>*
              renderMessageFromPackets(std::map<unsigned char,
                                                RobotisPacket>& p);
        private:
            std::map<PARAM_TABLE, 
                     std::map<unsigned char, RobotisPacket> > queue;
            RTT::os::Mutex qGuard;
    };

/*
    class Reader : public RTT::base::RunnableInterface {
        public:
            Reader(boost::asio::serial_port* p);
            bool initialize();
            void step();
            void loop();
            void finalize();
        //private:
            unsigned char rxBuf;
            boost::asio::serial_port* port;
            RTT::OutputPort< unsigned char > out;
    };
*/
    std::ostream &operator<<(std::ostream &out, const RobotisPacket &p);

    class Hardware : public FlexScanner::Hardware{
        public: 
            Hardware(std::string cfg, std::string args);
            virtual bool txBus(ACES::Message<unsigned char>* m);
    };

    /*
    class Hardware : public ACES::Hardware<unsigned char>{
        public:
            Hardware(std::string cfg, std::string args);
            virtual bool txBus(ACES::Message<unsigned char>* m);
            virtual void rxBus(int size=0);
            bool startHook();
            void stopHook();
            //static void rxHandle(//Hardware* This,
            //              const boost::system::error_code& error,
            //              int numBytes);

            //virtual bool processUS(ACES::Word<unsigned char>*);
            //virtual bool processDS(ACES::Message<unsigned char>*);
        protected:
            boost::asio::io_service io_service;
            boost::asio::serial_port port;
            //boost::asio::socket_base::bytes_readable hasData;
            RTT::InputPort<unsigned char> rxBuf;
            Reader rxReader;
            RTT::Activity rxActivity;
    };
    */

    class Protocol : public FlexScanner::Protocol<RobotisPacket, robotisFlex>{
        public:
            Protocol(std::string cfg, std::string args); 
            ACES::Message<unsigned char>*
              processDS(ACES::Word<RobotisPacket>* w);
            void txDSPending();
            ACES::Credentials* credFromPacket(RobotisPacket* p);
        private:
            bool triggerDS;
            RobotisQueue queue;
    };

/*
    class Protocol : public ACES::Protocol<unsigned char, RobotisPacket> {
        public:
            Protocol(std::string cfg, std::string args); 
            bool startHook();
            void stopHook();
            ACES::Word<RobotisPacket>* processUS(ACES::Word<unsigned char>* w);
            ACES::Message<unsigned char>*
              processDS(ACES::Word<RobotisPacket>* w);
            void txDSPending();

            //~Protocol();
            //ACES::Message* buildMessage(ACES::Credentials* cred);

            yyscan_t scanner;
            RobotisPacket* curPacket;
            unsigned char incoming_id;
            unsigned char incoming_len;
        private:
            bool triggerDS;
            RobotisQueue queue;
    };
*/

    class Device : public ACES::Device<float, RobotisPacket> {
        public:
            Device(std::string config, std::string args);
            //bool startHook();
            //void stopHook();
            virtual ACES::Word<RobotisPacket>* processDS(ACES::Word<float>*);
            virtual ACES::Word<float>* processUS(ACES::Word<RobotisPacket>*);
            bool getTable();
            bool setTable(int position, float val);
            float USScale(unsigned short in, int nodeID);
            unsigned short DSScale(float in, int nodeID);
        private:
            //!The memory table position of the last request issued
            int requestPos; 
            int requestLen; //!The size of data from the last issued request
            void printTable();
            unsigned char memTable[50];
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
    
    ACES::Message<unsigned char>* messageFromPacket(RobotisPacket* p);
    unsigned char checksum(RobotisPacket* p);
    template <class T>
    T limit(float c, T low, T high);
    unsigned short DSlimit(float d, unsigned short low, unsigned short high);
    bool appendParams( std::deque<unsigned char>* params,
                       unsigned short data, int size );
    int findTrueNodeID(int id, DIRECTION d);
};

 
//Function defs for the lexer
/*
extern "C" {
    int yylex ( yyscan_t yyscanner );
    //int yylex();
    int yylex_init( yyscan_t * ptr_yy_globals );
    int yylex_destroy(yyscan_t yyscanner);
    YY_EXTRA_TYPE yyget_extra(yyscan_t yyscanner);
    void yyset_extra (YY_EXTRA_TYPE p, yyscan_t yyscanner);
};
    YY_BUFFER_STATE yy_scan_bytes(const char* bytes, int len, yyscan_t yyscanner);
*/
#endif
