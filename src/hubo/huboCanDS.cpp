#include "huboCanDS.hpp"

namespace Hubo{

    canMsg::canMsg(): id(0), type(0), subType(0), r1(0), r2(0), r3(0), r4(0),
                      r5(0)
    {}

    canMsg::canMsg(unsigned long ID, huboCanType Type, cmdType st) :
        id(ID), type(Type), subType(st), r1(0), r2(0), r3(0), r4(0), r5(0){}

    canMsg::canMsg(unsigned long ID, huboCanType Type, cmdType st,
                   unsigned long R1, unsigned long R2,
                   unsigned long R3, unsigned long R4
                   unsigned long R5)
            : id(ID), type(Type), subType(st), r1(R1), r2(R2), r3(R3), r4(R4),
              r5(R5);

    /*
    TODO - Fix the printme() function
    void canMsg::printMe(){
        RTT::Logger::log() << "[" << id << "]: {" << flags << ", " << cob
                           << "} " << std::hex << "0x"
        //<< RTT::Logger::Debug << 
        for(int i = 0; i < length; i++){
            RTT::Logger:log() << data[i];
        }
            
        << std::dec << RTT::endlog()
    }
    */

    /* The bitStuffing algorithems are pulled directly from the original hubo
     * code base. They're wrong in that they use a sign bit instead of a two's
     * complement representation, but I'm maintaing them because they're needed
     * to ensure compatibility with the motor controllers */
    unsigned long bitStuff15byte(){
        if (bs < 0) return( (unsigned long)(((-bs) & 0x000007FF) | (1<<11)) );
        else	return( (unsigned long)(bs) );
    }

    unsigned long bitStuff3byte(long bs){
        if (bs < 0) return( (unsigned long)(((-bs) & 0x007FFFFF) | (1<<23)) );
        else	return( (unsigned long)(bs) );
    }

    canmsg_t* canMsg::toLineType(){
        canmsg_t* cm = new canmsg_t
        cm->flags = 0;
        cm->cob = 0;
        cm->id = id;
        cm->timestamp = 0;
        
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
                if(
                cm->length = 6;
                m1 = 
                cm->data[0] = m1Ref & 0x000000FF;
                cm->data[1] = (m1Ref & 0x0000FF00) >> 8;
                cm->data[2] = (m1Ref & 0x00FF0000) >> 16;
                cm->data[3] = m2Ref & 0x000000FF;
                cm->data[4] = (m2Ref & 0x0000FF00) >> 8;
                cm->data[5] = (m2Ref & 0x00FF0000) >> 16;
                break;
            //Inbound packet types are not allowed here, and represent an error
            case SENSOR_FT_RXDF:
            case SENSOR_AD_RXDF:
            case ENC_RXDF:
            case CUR_RXDF:
            case PM_RXDF:
            case STAT_RXDF:
            case STAT_RXDF:
            case NAME_RXDF:
            case DAOFFSET_RXDF:
            case ADOFFSET_RXDF:
            case OFFSET_RXDF:
            default:
                assert(false);
        }
        for(int i = 0; i < length; i++){
            cm->data[i] = data[i];
        }
        return cm;
    }

    canmsg_t* processCMD(canmsg_t* cm){
        switch(subtype){
        }
        return cm;
    }
} 
