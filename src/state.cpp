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
   
    void State<T>::updateHook(){
        if( samplingAttr.get() ){
            sample();
        }

        assert(dsQueue.size() < 100);
        while (dsQueue.size() ){
            SWord<T> h;
            dsQueue.dequeue(h);

            //{ RTT::OS::MutexLock lock(dsqGuard);
            //  h = dsQueue.front();
            //  dsQueue.pop_front();
            //}
            txDownStream(h);
        }
        
        assert(usQueue.size() < 100);
        while( usQueue.size() ){
            SWord<T> rx;
            usQueue.dequeue(rx);

            //ProtoResult* rx = NULL;
            //{ RTT::OS::MutexLock lock(usqGuard);
            //  rx = usQueue.front();
            //  usQueue.pop_front();
            //}
            RTT::Logger::log() << RTT::Logger::Debug << "(state) rxUS"
                               << RTT::endlog();
            //  RTT::Logger::log() <<  "r nid =" << rx->nodeID << " my nid="
            //                     << nodeIDAttr.get() << RTT::endlog();
            
            if(rx.getNodeID() == nodeID.get()){
                RTT::Logger::log() << RTT::Logger::Debug << "(state) assign"
                                   << RTT::endlog();
                asgnfunct(rx, this);
            }
        }
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
