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
            Credentials(int id);
            Credentials(std::string args);
            virtual void printme();
            int devID;
            //std::string wb_device_id;
    };
}
#endif
