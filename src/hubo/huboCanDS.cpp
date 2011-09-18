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

#include "huboCanDS.hpp"

namespace Hubo{

    canMsg::canMsg(): id(0), type(CAN_NONE), subType(CMD_NONE), r1(0), r2(0),
                      r3(0), r4(0), r5(0)
    {}

    canMsg::canMsg(unsigned long ID, huboCanType Type, cmdType st) :
        id(ID), type(Type), subType(st), r1(0), r2(0), r3(0), r4(0), r5(0){}

    canMsg::canMsg(unsigned long ID, huboCanType Type, cmdType st,
                   unsigned long R1, unsigned long R2,
                   unsigned long R3, unsigned long R4,
                   unsigned long R5)
            : id(ID), type(Type), subType(st), r1(R1), r2(R2), r3(R3), r4(R4),
              r5(R5){}

    unsigned long canMsg::getID(){
        return id;
    }

    huboCanType canMsg::getType(){
        return type;
    }

    cmdType canMsg::getCmd(){
        return subType;
    }

    unsigned long canMsg::getR1(){
        return r1;
    }

    unsigned long canMsg::getR2(){
        return r2;
    }

    unsigned long canMsg::getR3(){
        return r3;
    }

    unsigned long canMsg::getR4(){
        return r4;
    }

    unsigned long canMsg::getR5(){
        return r5;
    }

    void canMsg::printme(){
        std::ostringstream s(std::ostringstream::out);

        s << "[" << std::setbase(16) << id << "]{"
                 << std::setbase(16) << type << "/"
                 << std::setbase(16) << subType
                 << "} "; 
        s << "R1 = 0x" << std::hex << r1 << ", ";
        s << "R2 = 0x" << std::hex << r2 << ", ";
        s << "R3 = 0x" << std::hex << r3 << ", ";
        s << "R4 = 0x" << std::hex << r4 << ", ";
        s << "R5 = 0x" << std::hex << r5;
        RTT::Logger::log() << s.str() << RTT::endlog();
    }

    /* The bitStuffing algorithms are pulled directly from the original hubo
     * code base. They're wrong in that they use a sign bit instead of a two's
     * complement representation, but I'm maintaing them because they're needed
     * to ensure compatibility with the motor controllers */
    unsigned long canMsg::bitStuff15byte(long bs){
        if (bs < 0) return( (unsigned long)(((-bs) & 0x000007FF) | (1<<11)) );
        else	return( (unsigned long)(bs) );
    }

    unsigned long canMsg::bitStuff3byte(long bs){
        if (bs < 0) return( (unsigned long)(((-bs) & 0x007FFFFF) | (1<<23)) );
        else	return( (unsigned long)(bs) );
    }

    unsigned long canMsg::bitStuffCalibratePacket(long bs){
        if (bs < 0) return( (unsigned long)(((-bs) & 0x0007FFFF) | (1<<19)) );
        else	return( (unsigned long)(bs) );
    }

    unsigned char canMsg::bitStrip(unsigned long src, int byteNum){
        return (unsigned char)( (src >> (8*byteNum)) & 0x000000FFu );
    }
    
    /**
     * Fake Bitstuff algorithm for fingers (Rob).
     * Use a similar method to hubo lab to cram finger direction and PWM cycle
     * data into a single byte.
     */
    unsigned long canMsg::bitStuff1byte(long bs){
        if (bs < 0) return (unsigned long)(((-bs) & 0xF) | 0x10 );
        else	return (unsigned long)((bs) & 0xF );
    }

    canmsg_t* canMsg::toLineType(){
        canmsg_t* cm = new canmsg_t;
        cm->flags = 0;
        cm->cob = 0;
        //cm->timestamp = 0;
        
        switch(type){
            //How to construct the outbound packets
            case CMD_TXDF:
		cm->id = (unsigned char) type;
                processCMD(cm);
                break;
            case SEND_SENSOR_TXDF:
		cm->id = (unsigned char) type;
                cm->length = 1;
                cm->data[0] = 0;
                break;
            case REF_TXDF:
		cm->id = id + (unsigned char) type;
                processREF(cm);
                break;
            //Inbound packet types are not allowed here, and represent an error
            case SENSOR_FT_RXDF:
            case SENSOR_AD_RXDF:
            case ENC_RXDF:
            case CUR_RXDF:
            case PM_RXDF:
            case STAT_RXDF:
            case NAME_RXDF:
            case DAOFFSET_RXDF:
            case ADOFFSET_RXDF:
            case OFFSET_RXDF:
            default:
                assert(false);
        }
        return cm;
    }

    canmsg_t* canMsg::processCMD(canmsg_t* cm){
        cm->data[0] = (unsigned char)id;
        cm->data[1] = (unsigned char)subType;
        switch(subType){
            case SET_POS_GAIN_A:
            case SET_POS_GAIN_B:
            case SET_TRQ_GAIN_A:
            case SET_TRQ_GAIN_B:
                //For the gain setting related cases, r1 = Kp, r2 = Ki, r3 = Kd
                cm->data[2] = bitStrip(r1, 0);
                cm->data[3] = bitStrip(r1, 1);
                cm->data[4] = bitStrip(r2, 0);
                cm->data[5] = bitStrip(r2, 1);
                cm->data[6] = bitStrip(r3, 0);
                cm->data[7] = bitStrip(r3, 1);
                cm->length = 8;
                break;
            case HIP_ENABLE:
                cm->data[2] = 1;
                cm->length = 3; 
                break;
            case RUN_CMD:
                cm->length = 2;
                break;
            case NAME_INFO:
                //TODO: Test this
                //! r1 = command frequency (ms), r2-4 = "Limit" ch (0-2) (no idea what units are)
                cm->data[2] = bitStrip(r1,0);
                cm->data[3] = bitStrip(r2,0);
                cm->data[4] = bitStrip(r3,0);
                if (r5){
                    cm->data[5] = bitStrip(r4,0);
                    cm->length = 6;
                }
                else cm->length = 5;
                break;
            case BOARD_STATUS:
                //!Empty Packet
                cm->length = 2;
                break;
            case PWM_CMD:
                //PWM commands are all single bytes, regardless of the number of motors
                cm->data[2] = (unsigned char)r1;
                cm->data[3] = bitStrip(r2, 0);
                cm->data[4] = bitStrip(r2, 1);
                cm->data[5] = bitStrip(r3, 0);
                cm->data[6] = bitStrip(r3, 1);
                if(r5){
                    cm->length = 8;
                    cm->data[7] = bitStrip(r3, 2);
                }
                else cm->length = 7;
                break;
            case GO_LIMIT_POS:
                cm->data[2] = (unsigned char)r1; //Copy the mode bits
                cm->data[3] = bitStrip(r2, 1);
                cm->data[4] = bitStrip(r2, 0);
                cm->data[5] = bitStrip(r3, 1);
                cm->data[6] = bitStrip(r3, 0);

                if(r5){     //r5 is only populated in the case of 3ch ctrlr
                    cm->length = 7;    
                }
                else{
                    cm->data[7] = bitStrip(r4, 0);
                    cm->length = 8;
                }
                break;
            case NULL_CMD:
                cm->data[2] = (unsigned char)r1; //Copy the mode bits
                cm->length = 3;
                break;
            case GO_HOME:
            case CURR_LIMIT:
            case STOP_CMD:
            case CTRL_MODE:
            case SEND_ENC:
            case SEND_CURR:
            case SEND_PM:
            case ENC_ZERO:
            case SET_PERIOD_CMD:
            case SET_SAMPLE_CMD:
            case AD_READ_CMD:
            case DA_OUT_CMD:
            case DATA_TYPE_CMD:
            case DA_OFFSET_CMD:
            case AD_OFFSET_CMD:
            case OFFSET_CMD:
            default:
                assert(false);
                break;
        }
        return cm;
    }

    canmsg_t* canMsg::processREF(canmsg_t* cm){
        cm->length = 6;
        switch((int)subType){
            case 1:
            case 2:
                cm->data[0] = bitStrip(r1, 0);
                cm->data[1] = bitStrip(r1, 1);
                cm->data[2] = bitStrip(r1, 2);
                cm->data[3] = bitStrip(r2, 0);
                cm->data[4] = bitStrip(r2, 1);
                cm->data[5] = bitStrip(r2, 2);
                break;
            case 3:
                cm->data[0] = bitStrip(r1, 0);
                cm->data[1] = bitStrip(r1, 1);
                cm->data[2] = bitStrip(r2, 0);
                cm->data[3] = bitStrip(r2, 1);
                cm->data[4] = bitStrip(r3, 0);
                cm->data[5] = bitStrip(r3, 1);
                break;
            default:
                assert(false);
        }
        return cm;
    }
} 
