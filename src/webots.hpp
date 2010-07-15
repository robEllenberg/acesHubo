#ifndef ACES_WEBOTS_HPP
#define ACES_WEBOTS_HPP

#include <map>
#include <string>

#include <rtt/TaskContext.hpp>
#include <rtt/Logger.hpp>
#include <rtt/Activity.hpp>
#include <rtt/Ports.hpp>
#include <rtt/Event.hpp>

#include "svalue.hpp"
#include "protocol.hpp"
#include "state.hpp"
#include "hardware.hpp"
#include "taskcfg.hpp"
#include "credentials.hpp"

extern "C"{
    #include <webots/robot.h>
    #include <webots/servo.h>
}

namespace Webots{
    class Credentials;
    class Hardware : public ACES::Hardware {
        public:
            Hardware(std::string n, int pri, int UpdateFreq); 
            Hardware(ACES::taskCfg, std::string args);
            bool startHook();
            void updateHook();

            bool isBusy(void);
            bool transmit(ACES::Message* m);
            bool recieve();
            void setGoal();
            void step(int time=32);
            void stepRequest( std::map<std::string, void*>* );
            bool subscribeController(ACES::WbController* c);
    };
/*
    template <class T>
    class State : public ACES::State<T> {
        public:
            State(std::string n, ACES::Credentials* c,
                  int pri, int UpdateFreq);
            State(std::string pname, std::string cname,
                  int pri, int UpdateFreq,
                  float z=150.0, float rot=1.0);
            State(ACES::taskCfg cfg, ACES::Credentials* c, T ic);
            //void setGoal(std::map<std::string, ACES::SValue*>*);
            static ACES::Credentials* parseDispArgs(std::string type,
                                             std::string args);
    };
    #include "webots.cc"
*/   
    class Credentials : public ACES::Credentials {
        public:
            Credentials(Credentials* c);
            Credentials(std::string id_str, float z=150.0,
                        float dir=1.0);
            Credentials(char* id_str, float z=150.0, 
                        float dir=1.2);
            Credentials(Credentials* c, void* sp); 
            ACES::Credentials* copy(void* setP);
            static ACES::Credentials* parseDispArgs(std::string type,
                                             std::string args);
            void printme();
            std::string wb_device_id;
            float zero;
            float direction;
    };


    class Protocol : public ACES::Protocol {
        public:
            Protocol(std::string name, 
                     int pri, int UpdateFreq);
            Protocol(ACES::taskCfg cfg, std::string args);
            //ACES::Message* buildMessage(
            //                   Credentials* cred);
            //ACES::Credentials* parseHWInput(
            //                   ACES::Message* c) ;
            //void aggregateRequests(
            //    std::list<ACES::Credentials*> &reqs);

            //bool registerParam(ACES::State*);
            //std::map<std::string, bool> joints;
    };
}    

#endif
