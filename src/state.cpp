#include "state.hpp"
namespace ACES {
    ProtoState::ProtoState(std::string cfg, int nID) :
      taskCfg(cfg),
      RTT::TaskContext(name),
      nodeID("nodeID"),
      samplingAttr("sampling")
    {
        nodeID.set(nID);
        this->attributes()->addAttribute(&nodeID);
        this->attributes()->addAttribute(&samplingAttr);
        //Switching this default to false prevents automatic
        //sampling from starting
        samplingAttr.set(true);
    }

    std::string ProtoState::getName(){
        return name;
    }

    template <class T>
    void State<T>::sample(){
        RTT::Logger::log() << RTT::Logger::Debug << "SAMPLE (ori)!"
                           << RTT::endlog();
        Word<T>* s = new Word<T>(0, nodeID, 0, REFRESH);
        dsQueue.enqueue(s);
    }
   
    template <class T>
    void State<T>::rxUpStream(Word<T>* rx){
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
