#include "state.hpp"
namespace ACES {
    void State<T>::sample(){
        RTT::Logger::log() << RTT::Logger::Debug << "SAMPLE (ori)!"
                           << RTT::endlog();
        SWord<T> s(0, nodeID, 0, NULL, REFRESH);
        //Goal* g = new Goal(this->nodeID, REFRESH);

        //{ RTT::OS::MutexLock lock(dsqGuard);
        //  dsQueue.push_back(g);
        //}
        dsQueue.enqueue(s);
    }
   
    void State<T>::rxUpStream(SWord<T> rx){
        RTT::Logger::log() << "in:" << rx.getNodeID() << " my:"
                           << nodeID.get()
                           << " equiv: "
                           <<  (rx.getNodeID() == nodeID.get())
                           << RTT::endlog();
        if(rx.getNodeID() == nodeID.get())
        {
            usQueue.enqueue(rx);
            //RTT::OS::MutexLock lock(usqGuard);
            //usQueue.push_back(rx);
        }
    }

}
