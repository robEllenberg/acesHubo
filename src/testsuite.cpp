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

    Spinner::Spinner(std::string cfg, std::string args)
     : ACES::State<float>(cfg, 1){
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

        RTT::Logger::log() << RTT::Logger::Debug << "SAMPLE (spin)!"
                           << RTT::endlog();
        ACES::Word<float> *g = new ACES::Word<float>(val, this->nodeID.get(),
                                                     0, ACES::REFRESH);
        dsQueue.enqueue(g);
    }
};
