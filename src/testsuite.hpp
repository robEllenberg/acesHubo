#ifndef ACES_TESTSUITE_HPP
#define ACES_TESTSUITE_HPP

#include <string>
#include <sstream>

#include "hardware.hpp"
#include "device.hpp"
#include "protocol.hpp"
#include "state.hpp"

namespace TestSuite{

    class Hardware : public ACES::Hardware {
        public:
            Hardware(std::string cfg, std::string args);
            bool subscribeController(ACES::Controller* c);
    };
    
    class Device : public ACES::Device {
        public:
            Device(std::string cfg, std::string args);
    };

    class Protocol : public ACES::Protocol{
        public:
            Protocol(std::string cfg, std::string args);
    };

    class Spinner : public ACES::State<float>{
        public:
            Spinner(std::string cfg, std::string args);
            virtual void updateHook();
            float amp, dc, t;
    };
    
}

#endif
