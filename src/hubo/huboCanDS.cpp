#include "huboCanDS.hpp"

namespace Hubo{
    canMsg::canMsg() : flags(0), cob(0), id(0), length(8),
                       timestamp()
    {
        for(int i = 0; i < length; i++){
            data[i] = 0;
        }
    }

    canMsg::canMsg(unsigned long myid, short int len, unsigned char* d):
        flags(0), cob(0), id(myid), length(len), timestamp()
    {
        for(int i = 0; i < len; i++){
            data[i] = *(d+i);
        }
    }

    //canMsg::canMsg(unsigned long id, short int len, unsigned long d1,
    //       unsigned long d2);

    void canMsg::printMe(){
        RTT::Logger::log() << "[" << id << "]: {" << flags << ", " << cob
                           << "} " << std::hex << "0x"
        //<< RTT::Logger::Debug << 
        for(int i = 0; i < length; i++){
            RTT::Logger:log() << data[i];
        }}
            
        << std::dec << RTT::endlog()
    }

    canmsg_t* canMsg::toLineType(){
        canmsg_t* cm = new canmsg_t
        cm->flags = flags;
        cm->cob = cob;
        cm->id = id;
        cm->timestamp = timestamp;
        cm->length = length;
        
        for(int i = 0; i < length; i++){
            cm->data[i] = data[i];
        }
        return cm;
    }
} 
