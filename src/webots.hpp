#ifndef ACES_WEBOTS_HPP
#define ACES_WEBOTS_HPP

#include <map>
#include <string>

#include <rtt/TaskContext.hpp>
#include <rtt/Logger.hpp>
#include <rtt/Activity.hpp>
#include <rtt/Ports.hpp>
#include <rtt/Event.hpp>

#include "protocol.hpp"
#include "state.hpp"
#include "hardware.hpp"
#include "taskcfg.hpp"
#include "credentials.hpp"
#include "device.hpp"

extern "C"{
    #include <webots/robot.h>
    #include <webots/servo.h>
}

namespace Webots{
    class Hardware : public ACES::Hardware {
        public:
            Hardware(ACES::taskCfg, std::string args);
            bool startHook();
            void updateHook();

            bool isBusy(void);
            bool transmit(ACES::Message* m);
            bool recieve();
            void setGoal();
            void step(int time=32);
            void stepRequest( std::map<std::string, void*>* );
            bool subscribeController(ACES::Controller* c);
    };
   
    class Credentials : public ACES::Credentials {
        public:
            Credentials(Credentials* c);
            Credentials(std::string id_str, float z=150.0,
                        float dir=1.0);
            Credentials(char* id_str, float z=150.0, 
                        float dir=1.0);

            static ACES::Credentials* parseDispArgs(std::string args);
            void printme();

            std::string wb_device_id;
            float zero;
            float direction;
    };

    class Device : public ACES::Device {
        public:
            Device(std::string config, std::string args);
    };

    class Protocol : public ACES::Protocol {
        public:
            Protocol(std::string cfg, std::string args);
            //ACES::Message* buildMessage(
            //                   Credentials* cred);
            //ACES::Credentials* parseHWInput(
            //                   ACES::Message* c) ;
            //void aggregateRequests(
            //    std::list<ACES::Credentials*> &reqs);

            //bool registerParam(ACES::State*);
            //std::map<std::string, bool> joints;
    };

    class ScriptCtrl : public ACES::ScriptCtrl
    {
        public:
            ScriptCtrl(std::string config, std::string args);

            std::map<std::string, void*>* 
                    getStateVector(bool echo=0);
    };
}    

#endif
