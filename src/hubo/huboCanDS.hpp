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

#ifndef ACES_HUBOCAN_HPP
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
            static unsigned long bitStuff1byte(long bs);
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
