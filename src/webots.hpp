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
//#include "dispatcher.hpp"

extern "C"{
    #include <webots/robot.h>
    #include <webots/servo.h>
}

enum CREDTYPE { CRED_WB_JOINT=1 };

namespace Webots{
    
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
    };

    template <class T>
    class State : public ACES::State<T> {
        public:
            State(std::string n, ACES::Credentials<T>* c,
                  int pri, int UpdateFreq);
            State(std::string pname, std::string cname,
                  int pri, int UpdateFreq,
                  float z=150.0, float rot=1.0);
            State(ACES::taskCfg cfg, std::string args);
            //void setGoal(std::map<std::string, ACES::SValue*>*);
    };
   
    template <class T>
    class Credentials : public ACES::Credentials<T> {
        public:
            Credentials();
            Credentials(std::string id_str, float z=150.0,
                        float rot=1.0);
            Credentials(char* id_str, float z=150.0, 
                        float rot=1.0);
            Credentials(Credentials& c, ACES::SValue* p); 
            //ACES::Credentials<T>* credCopy(void* p=0);
            void printme();
            std::string wb_device_id;
            float zero;
            float rotation;
    };

    class Protocol : public ACES::Protocol {
        public:
            Protocol(std::string name, Hardware* hw,
                     int pri, int UpdateFreq);
            Protocol(taskCfg cfg, std::string args);
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
