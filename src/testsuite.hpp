#ifndef ACES_TESTSUITE_HPP
#define ACES_TESTSUITE_HPP

#include <string>
#include <sstream>

#include "hardware.hpp"
#include "device.hpp"
#include "protocol.hpp"
#include "state.hpp"
#include "word.hpp"

namespace TestSuite{

    template <typename T>
    class Hardware : public ACES::Hardware<T> {
        public:
            Hardware(std::string cfg, std::string args);
            bool subscribeController(ACES::Controller* c);
    };
    
    template <class S, class P>
    class Device : public ACES::Device<S,P> {
        public:
            Device(std::string cfg, std::string args);
    };

    template <class HW, class P>
    class Protocol : public ACES::Protocol<HW, P>{
        public:
            Protocol(std::string cfg, std::string args);
    };

    class Spinner : public ACES::State<float>{
        public:
            Spinner(std::string cfg, std::string args, bool sampling);
            virtual void sample();
            //virtual void updateHook();
            float amp, dc;
            RTT::os::TimeService::ticks t0;
    };
    
}

#endif
