#include "huboCanDS.hpp"

namespace Hubo{
    canMsg::canMsg() : flags(0), cob(0), id(0), length(0),
                       timestamp()
    {
        for(int i = 0; i < 8; i++){
            data[i] = 0;
        }
    }

    canMsg::canMsg(unsigned long myid, short int len, unsigned char* d):
        flags(0), cob(0), id(myid), length(len), timestamp()
    {
        for(int i = 0; i < 8; i++){
            data[i] = *(d+i);
        }
    }

    canMsg::canMsg(unsigned long id, short int len, unsigned long d1,
           unsigned long d2);

    void canMsg::printMe(){
        RTT::Logger::log() << "[" << id << "]: {" << flags << ", " << cob
                           << "} " << std::hex << "0x"
        //<< RTT::Logger::Debug << 
        for(int i = 0; i < 8; i++){
            RTT::Logger:log() << data[i];
        }}
            
        << std::dec << RTT::endlog()
    }

    canmsg_t canMsg::toDriverType(){
        canmsg_t cm = {flags, cob, id, timestamp, length, data};
        return cm;
    }
} 
