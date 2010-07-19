#ifndef ACES_CREDENTIALS_HPP
#define ACES_CREDENTIALS_HPP

#include <rtt/Logger.hpp>

namespace ACES {
/*
    class ProtoCredential{
        public:
            ProtoCredential(int t);
            int credType;
    };
*/
    class Credentials {
        public:
            virtual void printme() = 0;
    };
}
#endif
