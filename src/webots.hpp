#ifndef ACES_WEBOTS_HPP
#define ACES_WEBOTS_HPP

#include <map>
#include <string>

#include <rtt/TaskContext.hpp>
#include <rtt/Logger.hpp>
#include <rtt/Activity.hpp>
#include <rtt/Ports.hpp>
#include <rtt/Event.hpp>
#include <rtt/Buffer.hpp>

#include "protocol.hpp"
#include "state.hpp"
#include "hardware.hpp"
#include "taskcfg.hpp"
#include "credentials.hpp"
#include "device.hpp"
#include "word.hpp"

extern "C"{
    #include <webots/robot.h>
    #include <webots/servo.h>
}

namespace Webots{
    class Hardware : public ACES::Hardware {
        public:
            Hardware(ACES::taskCfg, std::string args);
            Hardware(std::string name);
            bool startHook();
            void updateHook();

            bool isBusy(void);
            bool transmit(ACES::Message* m);
            bool recieve();
            void setGoal();
            void step(int time=32);
            void stepRequest( std::map<std::string, void*>* );
            bool subscribeController(ACES::Controller* c);
            RTT::Buffer< ACES::ProtoWord* > *cache;
    };
   
    class Credentials : public ACES::Credentials {
        private:
            void assign(std::string id_str, std::string devname, float z,
                        float dir);
        public:
            Credentials(Credentials* c);
            Credentials(std::string args);
            Credentials(std::string id_str, std::string devname,
                        float z, float dir);

            void printme();

            std::string wb_device_id;
            float zero;
            float direction;
            std::string devName;
    };

    class Device : public ACES::Device {
        public:
            Device(std::string config, std::string args);
            Device(std::string name);
            void interpretResult(ACES::ProtoResult* rx);
            bool startHook();
            void stopHook();
    };

    class Protocol : public ACES::Protocol {
        public:
            Protocol(std::string cfg, std::string args);
            Protocol(std::string name);
            void interpretRx(ACES::ProtoWord* rx);
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
            ScriptCtrl(std::string name);

            std::map<std::string, void*>* 
                    getStateVector(bool echo=0);
    };

    class ArmCtrl : public ACES::ScriptCtrl
    {
        public:
            ArmCtrl(std::string config, std::string args);
            ArmCtrl(std::string name);
            std::map<std::string, void*>* 
                    getStateVector(bool echo=0);
    };
}    

#endif
