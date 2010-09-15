#include "word.hpp"

namespace ACES{
    ProtoResult::ProtoResult(int dev, int node, Credentials* c){
        devID = dev;
        nodeID = node;
        semiCred = c;
    }

    void ProtoResult::printme(){
        RTT::Logger::log() << "Result: devID=" << devID << " nodeID="
                           << nodeID << RTT::endlog();
    }
}
