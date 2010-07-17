#include "message.hpp"

namespace ACES {
    
/*
    Message::Message(std::list<Goal*> &gl){
        this->goalList = gl; 
    }
*/
    Message::Message(Goal* g) : goalList(1, g){
        
    }
/*
    Message::~Message(){
        while( goalList.size() ){
            //If the credential has a PVal it is a copy 
            //and needs to be deleted.

            //if( credList.back()->val ){
            if(true){
                delete goalList.back();
            }
           goalList.pop_back();
        }
    }
*/

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
