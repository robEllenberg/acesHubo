#ifndef ACES_CREDENTIALS_HPP
#define ACES_CREDENTIALS_HPP

#include <rtt/Logger.hpp>
#include "svalue.hpp"

namespace ACES {
    class ProtoCredential{
        public:
            ProtoCredential(int t);
            int credType;
    };

    template <class T>
    class Credentials : public ProtoCredential{
        public:
            //Credentials(const Credentials &c);
            //Credentials();
            Credentials(int id_num=0, T v=0, int type=0);
            Credentials(Credentials *c, T v);
            //virtual Credentials* credCopy(void* p=0) = 0;
            //~Credentials();
            virtual void printme() = 0;
            int id;
            T val;
    };

}
#endif
