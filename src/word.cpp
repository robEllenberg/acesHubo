#include "word.hpp"

namespace ACES{
    ProtoResult::ProtoResult(int dev, int node){
        devID = dev;
        nodeID = node;
    }

    void ProtoResult::printme(){
        RTT::Logger::log() << "Result: devID=" << devID << " nodeID="
                           << nodeID << RTT::endlog();
    }
}
