#ifndef ACES_PVALUE_HPP
#define ACES_PVALUE_HPP

#include <rtt/Logger.hpp>

namespace ACES {
    //!Abstract container for the value of a parameter
    class PValue {
        public:
            PValue(float v);
            //std::ostream &operator<<(std::ostream &out);
            virtual void printme() ;
            float val;
    };

    class charDevPValue : public PValue {
        public:
            unsigned char cval;
            charDevPValue(unsigned char c);
            void printme();
    };
} 


#endif
