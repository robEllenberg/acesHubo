#include "device.hpp"

namespace ACES{
    Device::Device(std::string config, std::string junk) :
      taskCfg(config),
      RTT::TaskContext(name),
      txDownStream("txDownStream"),
      txUpStream("txUpStream")
    {
        this->events()->addEvent(&txDownStream, "txDownStream", "goal",
                                 "The Goal/SP Data");
        this->events()->addEvent(&txUpStream, "txUpStream", "data",
                                 "Interperted data going to states");

        requestBuf = new RTT::Buffer<Goal*>(50);
        returnBuf = new RTT::Buffer<Goal*>(50);

        this->setActivity(
            //TODO - allow user to set priority
            new RTT::Activity( priority, 1.0/freq, 0, name )
        );
            
    }
/*
    Device::Device(std::string name) :
      RTT::TaskContext(name),
      txDownStream("txDownStream"),
      txUpStream("txUpStream")
    {
        this->events()->addEvent(&txDownStream, "txDownStream", "goal",
                                 "The Goal/SP Data");
        this->events()->addEvent(&txUpStream, "txUpStream", "data",
                                 "Interperted data going to states");

        requestBuf = new RTT::Buffer<Goal*>(50);
        returnBuf = new RTT::Buffer<Goal*>(50);
    }
*/
    void Device::rxDownStream(Goal* g){
        g->cred = credentials;
        requestBuf->Push(g);
    }

    void Device::attachCredentials(ACES::Credentials* c){
        credentials = c;
    }

    bool Device::subscribeState(ProtoState* s){
        this->connectPeers( (RTT::TaskContext*) s);
        //TODO - Figure out how calling this reception function
        //in a non-periodic thread affects RT characteristic
        RTT::Handle h = s->events()->setupConnection("txDownStream")
            .callback( this, &Device::rxDownStream
                     ,  s->engine()->events()
                     ).handle();
        if(!h.ready() ){
            return false;
        }
        h.connect();
        if(!h.connected() ){
            return false;
        }

        h = this->events()->setupConnection("txUpStream")
            .callback( s, &ProtoState::rxUpStream
                     ,  this->engine()->events()
                     ).handle();
        if(!h.ready() ){
            return false;
        }
        h.connect();
        if(!h.connected() ){
            return false;
        }

        return true;
    }

    bool Device::configureHook(){
        return true;
    }
/*
    bool Device::startHook(){
        return true;
    }
*/
    void Device::updateHook(){
        //Forward Path
        while(!requestBuf->empty() ){
            Goal* g;
            requestBuf->Pop(g);
            RTT::Logger::log() << "device sent" << RTT::endlog();
            txDownStream(g);
        }
        //Return Path
        while(! returnBuf->empty()){
            Goal* p;
            //Credentials* c = p->cred;
            returnBuf->Pop(p);
            //p->printme();
            txUpStream(p);
        }
    }
//    void Device::stopHook(){}
    void Device::cleanupHook(){}

}
