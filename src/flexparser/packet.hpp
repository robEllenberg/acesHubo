#ifndef __packet_h_
#define __packet_h_

#include <list>
#include <iostream>

class packet {
    public:
        packet();
        unsigned char id;
        unsigned char len;
        unsigned char error;
        std::list<unsigned char>* parameters;
        unsigned char checksum;
        void printme();
    };

#endif
