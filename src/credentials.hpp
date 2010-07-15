#ifndef ACES_CREDENTIALS_HPP
#define ACES_CREDENTIALS_HPP

#include <rtt/Logger.hpp>

namespace ACES {

    enum CREDTYPE { CRED_WB_JOINT=1 };

    class ProtoCredential{
        public:
            ProtoCredential(int t);
            //Non-Create Copy, to be called by a child class who
            //is implementing a copy of itself
            ProtoCredential* NCcopy(ProtoCredential* c);
            int credType;
    };

    class Credentials : public ProtoCredential{
        public:
            Credentials(Credentials* c);
            Credentials(int type);
            //MUST IMPLIMENT
            //virtual Credentials(Credentials* c, void* sp) = 0; 
            virtual void printme() = 0;
            virtual Credentials* copy(void* setP) = 0;

            void* setPoint;
            //int id;
    };
}
#endif
