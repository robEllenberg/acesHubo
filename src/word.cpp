#include "word.hpp"

namespace ACES{
/*
    ProtoResult::ProtoResult(int node, Credentials* c){
        //devID = dev;
        nodeID = node;
        semiCred = c;
    }

    void ProtoResult::printme(){
        RTT::Logger::log() << " nodeID="
                           << nodeID << RTT::endlog();
    }
*/
    template <class T>
    int PDWord<T>::getNodeID(){
        return nodeID;
    }

    template <class T>
    int PDWord<T>::getDevID(){
        return devID;
    }

    template <class T>
    Credentials* PDWord<T>::getCred(){
        return cred;
    }

    template <class T>
    int SWord<T>::getMode(){
        return mode;
    }
}
