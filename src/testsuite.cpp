#include "testsuite.hpp"

namespace TestSuite{
    template <class T>
    Hardware<T>::Hardware(std::string cfg, std::string args)
      : ACES::Hardware<T>(cfg, args)
    { }
   
    template <class T>
    bool Hardware<T>::subscribeController(ACES::Controller* c){
        return true;
    }

    template <class S, class P>
    Device<S,P>::Device(std::string name, std::string args)
      : ACES::Device<S,P>(name)
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
        ACES::Goal* g = new ACES::Goal(this->nodeID, ACES::REFRESH,
                                       new float(val));
        { RTT::OS::MutexLock lock(dsqGuard);
          dsQueue.push_back(g);
        }
    }
};
