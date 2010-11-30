#include "testsuite.hpp"

namespace TestSuite{
    template <class T>
    Hardware<T>::Hardware(std::string cfg, std::string args)
      : ACES::Hardware<T>(cfg, args)
    { }
   
    template <class S, class PD>
    Device<S,PD>::Device(std::string cfg, std::string args)
      : ACES::Device<S,PD>(cfg)
    {
        this->credentials = new ACES::Credentials(args);
    }
    
    template <class HW, class P>
    Protocol<HW,P>::Protocol(std::string cfg, std::string args)
      : ACES::Protocol<HW,P>(cfg, args)
    { }

    Spinner::Spinner(std::string cfg, std::string args, bool sampling)
     : ACES::State<float>(cfg, 1, sampling){
        std::istringstream s1(args, std::istringstream::in);
        float high, low, amp, dc;
        s1 >> low >> high;
        amp = (high - low) / 2.0;
        dc = (high + low) / 2.0;
        this->amp = amp;
        this->dc = dc;
        t0 = RTT::os::TimeService::Instance()->getTicks();
    }
    
    void Spinner::sample(){
        float t = RTT::os::TimeService::Instance()->secondsSince(t0);
        float val = amp*sin(t) + dc;

        RTT::Logger::log() << RTT::Logger::Debug << "(State: " << name
                           << ") sample: " << val 
                           << RTT::endlog();
        ACES::Word<float> *g = new ACES::Word<float>(val, this->nodeID,
                                                     0, ACES::REFRESH);
        txDownStream.write(g);
    }

    //TODO - Total Kludge, there's got to be a 'proper' way to get the compiler
    //to generate the specializations we want and throw them into the library
    void forcegenerate(){
        Device<float, float> d("a", "b");
        Hardware<float> h("a", "b");
        Protocol<float, float> p("e", "f");
    }
};
