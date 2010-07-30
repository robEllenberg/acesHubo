#include "device.hpp"

namespace ACES{
    Device::Device(std::string config, int junk) :
      taskCfg(config),
      RTT::TaskContext(name),
      TxRequest("TxRequest"),
      announceData("announceData")
    {
        this->events()->addEvent(&TxRequest, "TxRequest", "goal",
                                 "The Goal/SP Data");
        this->events()->addEvent(&announceData, "announceData", "data",
                                 "Interperted data going to states");

        requestBuf = new RTT::Buffer<Goal*>(50);
        returnBuf = new RTT::Buffer<Goal*>(50);

        this->setActivity(
            //TODO - allow user to set priority
            new RTT::Activity( priority, 1.0/freq, 0, name )
        );
            
    }

    Device::Device(std::string name) :
      RTT::TaskContext(name),
      TxRequest("TxRequest"),
      announceData("announceData")
    {
        this->events()->addEvent(&TxRequest, "TxRequest", "goal",
                                 "The Goal/SP Data");
        this->events()->addEvent(&announceData, "announceData", "data",
                                 "Interperted data going to states");

        requestBuf = new RTT::Buffer<Goal*>(50);
        returnBuf = new RTT::Buffer<Goal*>(50);
    }

    void Device::RxGoal(Goal* g){
        g->cred = credentials;
        /*if(g->mode == SET){
            g->printme();
        }*/
        //g->cred->printme();
        requestBuf->Push(g);
        //TxRequest(g);
    }

    void Device::attachCredentials(ACES::Credentials* c){
        credentials = c;
    }

    bool Device::subscribeState(ProtoState* s){
        this->connectPeers( (RTT::TaskContext*) s);
        //TODO - Figure out how calling this reception function
        //in a non-periodic thread affects RT characteristic
        RTT::Handle h = s->events()->setupConnection("announceGoal")
            .callback( this, &Device::RxGoal
                     ,  s->engine()->events()
                     ).handle();
        if( not h.ready() ){
            return false;
        }
        h.connect();
        if( not h.connected() ){
            return false;
        }

        h = this->events()->setupConnection("announceData")
            .callback( s, &ProtoState::RxData
                     ,  this->engine()->events()
                     ).handle();
        if( not h.ready() ){
            return false;
        }
        h.connect();
        if( not h.connected() ){
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
        while( not requestBuf->empty() ){
            Goal* g;
            requestBuf->Pop(g);
            TxRequest(g);
        }
        //Return Path
        while(not returnBuf->empty()){
            Goal* p;
            //Credentials* c = p->cred;
            returnBuf->Pop(p);
            //p->printme();
            announceData(p);
        }
    }
//    void Device::stopHook(){}
    void Device::cleanupHook(){}

}
