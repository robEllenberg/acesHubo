#ifndef ACES_CREDENTIALS_HPP
#define ACES_CREDENTIALS_HPP

#include <rtt/Logger.hpp>
#include "pvalue.hpp"

namespace ACES {
 
    class Credentials{
        public:
            //Credentials(const Credentials &c);
            //Credentials();
            Credentials(int id_num=0, PValue* p=0);
            Credentials(Credentials *c, PValue* p=0);
            virtual Credentials* credCopy(PValue* p=0) = 0;
            ~Credentials();
            virtual void printme();
            int id;
            PValue* val;
    };

}
#endif
