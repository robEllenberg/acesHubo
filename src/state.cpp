#include "state.hpp"
namespace ACES {
    ProtoState::ProtoState(std::string cfg, int nID) :
      taskCfg(cfg),
      RTT::TaskContext(name),
      nodeID(nID),
      //Switching this default to false prevents automatic
      //sampling from starting
      samplingAttr(true),
      intEnable(true),
      diffEnable(true),
      diffThreshold(10e-6)
    {
        //nodeID.set(nID);
        //samplingAttr.set(true);
        this->addAttribute("NodeID", nodeID);
        this->addAttribute("sampling", samplingAttr);
        this->addAttribute("intEnable", intEnable);
        this->addAttribute("diffEnable", diffEnable);
        this->addAttribute("diffThreshold", diffThreshold);
    }

    bool ProtoState::subscribeController(RTT::TaskContext* c){
        this->connectPeers( (RTT::TaskContext*) c);

        RTT::base::PortInterface *myPort = NULL, *theirPort=NULL;
        bool success;
        RTT::ConnPolicy policy = RTT::ConnPolicy::buffer(200);
        
        theirPort = (RTT::base::PortInterface*)c->ports()->getPort("TxDS");
        myPort = (RTT::base::PortInterface*)this->ports()->getPort("RxDS");
        success = theirPort->connectTo(myPort, policy);
        if(not success){
            return false;
        }

        /*RTT::Handle h = c->events()->setupConnection("applyStateVector")
                .callback( this, &State<T>::rxDownStream
                          , c->engine()->events()
                        ).handle();*/
        return true;
    }
   
/*
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
*/
}
