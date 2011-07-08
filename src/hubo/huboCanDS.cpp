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

    /* The bitStuffing algorithems are pulled directly from the original hubo
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

    canmsg_t* canMsg::toLineType(){
        canmsg_t* cm = new canmsg_t;
        cm->flags = 0;
        cm->cob = 0;
        //cm->timestamp = 0;
        cm->id = (unsigned char) type;
        
        switch(type){
            //How to construct the outbound packets
            case CMD_TXDF:
                processCMD(cm);
                break;
            case SEND_SENSOR_TXDF:
                cm->length = 1;
                cm->data[0] = 0;
                break;
            case REF_TXDF:
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
            case BOARD_STATUS:
            case SEND_ENC:
            case SEND_CURR:
            case SEND_PM:
            case ENC_ZERO:
            case GO_HOME:
            case PWM_CMD:
            case STOP_CMD:
            case CTRL_MODE:
                assert(false);
                break;
            case GO_LIMIT_POS:
                cm->data[2] = (unsigned char)r1; //Copy the mode bits
                if(r3){     //r3 is only populated in the case of Neck or Wrist
                    cm->data[3] = bitStrip(r2, 0);
                    cm->data[4] = bitStrip(r2, 1);
                    cm->data[5] = bitStrip(r3, 0);
                    cm->data[6] = bitStrip(r4, 0);
                    cm->length = 7;
                }
                else{
                    //Use the mode bits to determine the channel
                    switch( r1 & 0x30 ){  
                        case 0x10: //Channel 1
                            cm->data[3] = bitStrip(r2, 1);
                            cm->data[4] = bitStrip(r2, 0);
                            cm->data[5] = 0;
                            cm->data[6] = 0;
                            cm->data[7] = (unsigned char)((r2 >> 16) & 0xF);
                            break;
                        case 0x20: //Channel 2
                            cm->data[3] = 0;
                            cm->data[4] = 0;
                            cm->data[5] = bitStrip(r2, 1);
                            cm->data[6] = bitStrip(r2, 0);
                            cm->data[7] =(unsigned char) (((r2>>16) & 0xF)<<4);
                            break;
                        default:
                            assert(false);
                    }
                    cm->length = 8;
                }
                break;
            case CURR_LIMIT:
            case NULL_CMD:
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
