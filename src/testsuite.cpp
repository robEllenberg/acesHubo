#include "testsuite.hpp"

namespace TestSuite{
    Hardware::Hardware(std::string cfg, std::string args)
      : ACES::Hardware(cfg, args)
    { }

//    Hardware::Hardware(std::string name)
//      : ACES::Hardware(name)
//    { }

    bool Hardware::transmit(ACES::Message* m){
        //m->printme();
        RTT::Logger::log() << "Update" << RTT::endlog();
        return true;
    }

    bool Hardware::recieve(){
        return true;
    }
    
    bool Hardware::subscribeController(ACES::Controller* c){
        return true;
    }
    
    void Hardware::updateHook(){
        //RTT::Logger::log() << "HW(" << name << ") Tick" << RTT::endlog();
    }
    
    void Credentials::printme(){
        RTT::Logger::log() << "This is a credential." << RTT::endlog();
    }

    Device::Device(std::string cfg, std::string args)
      : ACES::Device(cfg, args)
    { }
/*
    Device::Device(std::string name)
      : ACES::Device(name)
    { }
*/    
    void Device::interpretResult (ACES::ProtoResult *rx){
        //ACES::Result<ACES::Goal*>* r = (ACES::Result<ACES::Goal*>*)rx;
        //ACES::Goal* g = r->result;
        //Credentials* c = (Credentials*) g->cred;
        
        //If our name matches the name on the packet, this one's for us,
        //pass it along to the States - let them sort it out
        //if( (c->devName) == getName()){
        //    returnBuf->Push(g);
        //}
    }
    
    Protocol::Protocol(std::string cfg, std::string args)
      : ACES::Protocol(cfg, args)
    { }
/*
    Protocol::Protocol(std::string name)
      : ACES::Protocol(name)
    { }
*/     
    void Protocol::interpretRx(ACES::ProtoWord *){
    }

    //void Protocol::addRequest(Goal*){
    //}
};
