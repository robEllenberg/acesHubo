#ifndef ACES_PVALUE_HPP
#define ACES_PVALUE_HPP

#include <rtt/Logger.hpp>

namespace ACES {
    //!Abstract container for the value of a parameter
    class SValue {
        public:
            SValue(float v);
            //std::ostream &operator<<(std::ostream &out);
            virtual void printme() ;
            float val;
    };

    class charDevSValue : public SValue {
        public:
            unsigned char cval;
            charDevSValue(unsigned char c);
            void printme();
    };
} 


#endif
