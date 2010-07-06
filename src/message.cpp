#include "message.hpp"

namespace ACES {
    
    Message::Message(std::list<void*> &cl){
        this->credList = cl; 
    }

    Message::Message(void* c) : credList(1, c){
        
    }

    Message::~Message(){
        while( credList.size() ){
            //If the credential has a PVal it is a copy 
            //and needs to be deleted.

            //if( credList.back()->val ){
            if(true){
                delete credList.back();
            }
            credList.pop_back();
        }
    }

/*
    void Message::printme(){
        RTT::Logger::log() << "Message:"
                           << RTT::Logger::endl;
        std::list<void*>::iterator it;
        for(it = credList.begin();
            it != credList.end();
            it++){
                RTT::Logger::log() << "\t";
                (*it)->printme();
        }
    }
*/

/*
    charDevMessage::charDevMessage(){
        std::list<unsigned char> lineRepresentation(0);
    }
    
    charDevMessage::charDevMessage(std::list<unsigned char>& m){
        this->lineRepresentation = m;
    }
*/
    
}
