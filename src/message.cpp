#include "message.hpp"

namespace ACES {
    
    Message::Message(std::list<ACES::Credentials*> &cl){
        this->credList = cl; 
    }

    Message::Message(Credentials* c) : credList(1, c){
        
    }

    Message::~Message(){
        while( credList.size() ){
            //If the credential has a PVal it is a copy 
            //and needs to be deleted.
            if( credList.back()->val ){
                delete credList.back();
            }
            credList.pop_back();
        }
    }

    void Message::printme(){
        RTT::Logger::log() << "Message:"
                           << RTT::Logger::endl;
        std::list<ACES::Credentials*>::iterator it;
        for(it = credList.begin();
            it != credList.end();
            it++){
                RTT::Logger::log() << "\t";
                (*it)->printme();
        }
    }

/*
    charDevMessage::charDevMessage(){
        std::list<unsigned char> lineRepresentation(0);
    }
    
    charDevMessage::charDevMessage(std::list<unsigned char>& m){
        this->lineRepresentation = m;
    }
*/
    
}
