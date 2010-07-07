#ifndef ACES_TESTSUITE_HPP
#define ACES_TESTSUITE_HPP

#include "hardware.hpp"
//#include "credentials.hpp"
#include "protocol.hpp"
#include "message.hpp"

namespace TestSuite{

    class Hardware : public ACES::Hardware {
        public:
            //void updateHook();
            Hardware(std::string name, int priority,
                     int UpdateFreq);
            bool isBusy(void);
            bool transmit(ACES::Message* m);
            bool recieve();
    };

    class Protocol : public ACES::Protocol{
        public:
            Protocol(std::string name, ACES::Hardware* hw,
                             int pri, int UpdateFreq);
            ACES::Credentials* parseHWInput(
                               ACES::Message* c) ;
            void aggregateRequests(
                std::list<ACES::Credentials*> &reqs);
    };

    class State : public ACES::State{
        public:
            State(std::string n, ACES::Credentials* c,
              int pri, int UpdateFreq);
            //void set(float setPoint);
            //bool setGoal(ACES::SValue* val);
            //bool goalSet();
    };
}

#endif
