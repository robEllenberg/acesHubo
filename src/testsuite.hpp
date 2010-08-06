#ifndef ACES_TESTSUITE_HPP
#define ACES_TESTSUITE_HPP

#include <string>
#include <sstream>

#include "hardware.hpp"
//#include "credentials.hpp"
#include "protocol.hpp"
#include "message.hpp"

namespace TestSuite{

    class Hardware : public ACES::Hardware {
        public:
            Hardware(std::string cfg, std::string args);
            //Hardware(std::string name);
            
            //void updateHook();
            
            //bool transmit(ACES::Message* m);
            //bool recieve();
            bool subscribeController(ACES::Controller* c);
    };
    
/*
    class Credentials : public ACES::Credentials {
        public:
            void printme();
    };
*/    
    class Device : public ACES::Device {
        public:
            Device(std::string cfg, std::string args);
            //Device(std::string name);
            //void interpretResult (ACES::ProtoResult *rx);    
    };

    class Protocol : public ACES::Protocol{
        public:
            Protocol(std::string cfg, std::string args);
            //Protocol(std::string name);            
            //void interpretRx(ACES::ProtoWord *);
    };

    class Spinner : public ACES::State<float>{
        public:
            Spinner(std::string cfg, std::string args);
            virtual void updateHook();
            float high, low, t;
    };
    
}

#endif
