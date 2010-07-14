#ifndef ACES_CREDENTIALS_HPP
#define ACES_CREDENTIALS_HPP

#include <rtt/Logger.hpp>

namespace ACES {

    enum CREDTYPE { CRED_WB_JOINT=1 };

    class ProtoCredential{
        public:
            ProtoCredential(int t);
            int credType;
    };

    class Credentials : public ProtoCredential{
        public:
            Credentials(Credentials* c);
            Credentials(int type, int id_num=0);
            virtual void printme() = 0;

            int id;
            void* setPoint;
    };
}
#endif
