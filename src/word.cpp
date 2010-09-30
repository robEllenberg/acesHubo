#include "word.hpp"

namespace ACES{
    ProtoResult::ProtoResult(int node, Credentials* c){
        //devID = dev;
        nodeID = node;
        semiCred = c;
    }

    void ProtoResult::printme(){
        RTT::Logger::log() << " nodeID="
                           << nodeID << RTT::endlog();
    }
}
