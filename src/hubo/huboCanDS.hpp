#ifndef ACES_HUBOCANi_HPP
#define ACES_HUBOCAN_HPP

#include <iostream>
#include <iomanip>
#include <sstream>
#include <sys/time.h>
#include <assert.h>
#include <rtt/Logger.hpp>
#include "can4linux.h"  //Include the can4linux data structures

//For reference, the canmsg_t data structure from can4linux
/*
typedef struct {
    // flags, indicating or controlling special message properties 
    int             flags;
    int             cob;	     // < CAN object number, used in Full CAN  
    unsigned   long id;		     // < CAN message ID, 4 bytes  
    struct timeval  timestamp;	 // < time stamp for received messages 
    short      int  length;	     // < number of bytes in the CAN message 
    unsigned   char data[CAN_MSG_LENGTH]; // < data, 0...8 bytes 
} canmsg_t;
*/

//CAN Notes
/*
    High ID -> Low Priority
    Low ID -> High Priority
*/

namespace Hubo{
    enum huboCanType { CAN_NONE = 0x0, CMD_TXDF = 0x1, SEND_SENSOR_TXDF = 0x2,
                       REF_TXDF = 0x10, 
                       SENSOR_FT_RXDF = 0x40, SENSOR_AD_RXDF = 0x50,
                       ENC_RXDF = 0x60, CUR_RXDF = 0x90, PM_RXDF = 0x120,
                       STAT_RXDF = 0x150, NAME_RXDF = 0x190,
                       DAOFFSET_RXDF = 0x310, ADOFFSET_RXDF = 0x320,
                       OFFSET_RXDF = 0x330 };
                       
    enum cmdType { CMD_NONE = 0x0, NAME_INFO = 0x1, BOARD_STATUS, SEND_ENC,
                   SEND_CURR, 
                   SEND_PM, ENC_ZERO, SET_POS_GAIN_A, SET_POS_GAIN_B,
                   SET_TRQ_GAIN_A, SET_TRQ_GAIN_B, HIP_ENABLE, GO_HOME,
                   PWM_CMD, RUN_CMD, STOP_CMD, CTRL_MODE, GO_LIMIT_POS,
                   CURR_LIMIT, NULL_CMD = 0x81, SET_PERIOD_CMD, SET_SAMPLE_CMD,
                   AD_READ_CMD, DA_OUT_CMD, DATA_TYPE_CMD, DA_OFFSET_CMD,
                   AD_OFFSET_CMD, OFFSET_CMD };
                       
    class canMsg {
        public:
            canMsg();
            canMsg(unsigned long id, huboCanType type, cmdType subType);
            canMsg(unsigned long id, huboCanType type, cmdType subType,
                   unsigned long r1, unsigned long r2,
                   unsigned long r3, unsigned long r4,
                   unsigned long r5);
            unsigned long getID();
            huboCanType getType();
            cmdType getCmd();

            unsigned long getR1();
            unsigned long getR2();
            unsigned long getR3();
            unsigned long getR4();
            unsigned long getR5();

            void printme();
            static unsigned long bitStuff15byte(long bs);
            static unsigned long bitStuff3byte(long bs);
            static unsigned long bitStuffCalibratePacket(long bs);
            static unsigned long unpack2byte();
            static unsigned long unpack4byte();
            static unsigned char bitStrip(unsigned long src, int byteNum);
            canmsg_t* toLineType();
        private:                       //   [bits] Meaning
            canmsg_t* processCMD(canmsg_t*);
            canmsg_t* processREF(canmsg_t*);

            unsigned long id;
            huboCanType type;
            cmdType subType;

            unsigned long r1;  //multi-purpose configuration registers
            unsigned long r2;
            unsigned long r3;
            unsigned long r4;
            unsigned long r5;
    };
}
#endif
