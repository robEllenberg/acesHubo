#include "testsuite.hpp"

namespace TestSuite{
    Hardware::Hardware(std::string cfg, std::string args)
      : ACES::Hardware(cfg, args)
    { }
   
    bool Hardware::subscribeController(ACES::Controller* c){
        return true;
    }

    Device::Device(std::string name, std::string args)
      : ACES::Device(name)
    {
        credentials = new ACES::Credentials(args);
    }
    
    Protocol::Protocol(std::string cfg, std::string args)
      : ACES::Protocol(cfg, args)
    { }

    Spinner::Spinner(std::string cfg, std::string args)
     : ACES::State<float>::State(cfg, 1){
        std::istringstream s1(args, std::istringstream::in);
        float high, low, amp, dc;
        s1 >> low >> high;
        amp = (high - low) / 2.0;
        dc = (high + low) / 2.0;
        this->amp = amp;
        this->dc = dc;
        t = 0;
    }
    
    void Spinner::sample(){
        float val = amp*sin(t) + dc;
        t += freq;
        //RTT::Logger::log() << val <<RTT::endlog();

        RTT::Logger::log() << RTT::Logger::Debug << "SAMPLE (spin)!" << RTT::endlog();
        ACES::Goal* g = new ACES::Goal(this->nodeID, ACES::REFRESH, new float(val));
        set_stack->Push(g);
        /*
        while (!set_stack->empty() ){
            ACES::Goal* h;
            set_stack->Pop(h);
            txDownStream(h);
        }
        */
    }
};
