#ifndef ACES_MESSAGE_HPP
#define ACES_MESSAGE_HPP

#include <list>
#include <rtt/Logger.hpp>
#include "credentials.hpp"
#include "goal.hpp"

namespace ACES{
    
    class Message {
        public:
            //TODO - All these void* should be too credentials
            //Message(std::list<Goal*> &gl);
            Message(Goal* g);
            ~Message();
            //void printme();
            std::list<Goal*> goalList;
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
