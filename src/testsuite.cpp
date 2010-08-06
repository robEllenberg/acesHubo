#include "testsuite.hpp"

namespace TestSuite{
    Hardware::Hardware(std::string cfg, std::string args)
      : ACES::Hardware(cfg, args)
    { }

//    Hardware::Hardware(std::string name)
//      : ACES::Hardware(name)
//    { }

/*
    bool Hardware::transmit(ACES::Message* m){
        //m->printme();
        RTT::Logger::log() << "Update" << RTT::endlog();
        return true;
    }

    bool Hardware::recieve(){
        return true;
    }
*/   
    bool Hardware::subscribeController(ACES::Controller* c){
        return true;
    }
/*    
    void Hardware::updateHook(){
        //RTT::Logger::log() << "HW(" << name << ") Tick" << RTT::endlog();
    }
*/    

/*
    Device::Device(std::string cfg, std::string args)
      : ACES::Device(cfg, args)
    { }
*/
    Device::Device(std::string name, std::string args)
      : ACES::Device(name, args)
    {
        std::istringstream s1(args, std::istringstream::in);
        int idnum;
        s1 >> idnum;
        credentials = new ACES::Credentials(idnum);
    }
//    void Device::interpretResult (ACES::ProtoResult *rx){
        //ACES::Result<ACES::Goal*>* r = (ACES::Result<ACES::Goal*>*)rx;
        //ACES::Goal* g = r->result;
        //Credentials* c = (Credentials*) g->cred;
        
        //If our name matches the name on the packet, this one's for us,
        //pass it along to the States - let them sort it out
        //if( (c->devName) == getName()){
        //    returnBuf->Push(g);
        //}
//    }
    
    Protocol::Protocol(std::string cfg, std::string args)
      : ACES::Protocol(cfg, args)
    { }
/*
    Protocol::Protocol(std::string name)
      : ACES::Protocol(name)
    { }
*/     
/*
    void Protocol::interpretRx(ACES::ProtoWord *){
    }
*/
    //void Protocol::addRequest(Goal*){
    //}
    Spinner::Spinner(std::string cfg, std::string args)
     : ACES::State<float>::State(cfg, args){
        std::istringstream s1(args, std::istringstream::in);
        float high, low;
        s1 >> low >> high;
        this->high = high;
        this->low = low;
        t = 0;
    }
    
    void Spinner::updateHook(){
        float amp = high - low;
        float dc = (high + low) / 2.0;
        float val = amp*sin(t) + dc;
        t += freq;

        ACES::Goal* g = new ACES::Goal(this->nodeID, ACES::REFRESH, new float(val));
        set_stack->Push(g);
        while (!set_stack->empty() ){
            ACES::Goal* h;
            set_stack->Pop(h);
            txDownStream(h);
        }
    }
};
