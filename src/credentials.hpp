#ifndef ACES_CREDENTIALS_HPP
#define ACES_CREDENTIALS_HPP

#include <rtt/Logger.hpp>

namespace ACES {

    class Credentials {
        public:
            Credentials(int id);
            Credentials(std::string args);
            virtual void printme();
            virtual bool operator==(Credentials& other);
            int devID;
    };
}
#endif
