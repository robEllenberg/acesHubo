#ifndef ACES_MESSAGE_HPP
#define ACES_MESSAGE_HPP

#include <list>
#include <rtt/Logger.hpp>
#include "credentials.hpp"

namespace ACES{
    
    class Message {
        public:
            //TODO - All these void* should be too credentials
            Message(std::list<void*> &cl);
            Message(void* c);
            ~Message();
            //void printme();
            std::list<void*> credList;
    };
/*
    class charDevMessage : Message {
        public:
            charDevMessage();
            charDevMessage(std::list<unsigned char>& m);
            std::list<unsigned char> lineRepresentation;
    }
*/
    
}    
#endif
