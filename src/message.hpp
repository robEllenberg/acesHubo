#ifndef ACES_MESSAGE_HPP
#define ACES_MESSAGE_HPP

#include <list>
#include <rtt/Logger.hpp>
#include "credentials.hpp"

namespace ACES{
    
    class Message {
        public:
            Message(std::list<Credentials*> &cl);
            Message(Credentials* c);
            ~Message();
            void printme();
            std::list<ACES::Credentials*> credList;
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
