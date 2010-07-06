#ifndef ACES_WEBOTS_HPP
#define ACES_WEBOTS_HPP

#include <map>
#include <string>

#include <rtt/TaskContext.hpp>
#include <rtt/Logger.hpp>
#include <rtt/Activity.hpp>
#include <rtt/Ports.hpp>
#include <rtt/Event.hpp>

#include "pvalue.hpp"
#include "protocol.hpp"
#include "parameter.hpp"
#include "dispatcher.hpp"

extern "C"{
    #include <webots/robot.h>
    #include <webots/servo.h>
}

namespace Webots{
    
    class Hardware : public ACES::Hardware {
        public:
            Hardware(std::string n, int pri, int UpdateFreq); 
            bool startHook();
            void updateHook();

            bool isBusy(void);
            bool transmit(ACES::Message* m);
            bool recieve();
            void setGoal();
            void step(int time=32);
    };

    class Parameter : public ACES::Parameter {
        public:
            Parameter(std::string n,
              ACES::Credentials* c,
              int pri, int UpdateFreq);
            Parameter(std::string pname, std::string cname,
              int pri, int UpdateFreq,
              float z=150.0, float rot=1.0);
            void setGoal(std::map<std::string, ACES::PValue*>*);
    };
   
    class Credentials : public ACES::Credentials {
        public:
            Credentials();
            Credentials(std::string id_str, float z=150.0,
                        float rot=1.0);
            Credentials(char* id_str, float z=150.0, 
                        float rot=1.0);
            Credentials(Credentials& c, ACES::PValue* p); 
            ACES::Credentials* credCopy(ACES::PValue* p=0);
            void printme();
            std::string wb_device_id;
            float zero;
            float rotation;
    };

    class Protocol : public ACES::Protocol {
        public:
            Protocol(std::string name, Hardware* hw,
                     int pri, int UpdateFreq);
            //ACES::Message* buildMessage(
            //                   Credentials* cred);
            ACES::Credentials* parseHWInput(
                               ACES::Message* c) ;
            void aggregateRequests(
                std::list<ACES::Credentials*> &reqs);

            //bool registerParam(ACES::Parameter*);
            //std::map<std::string, bool> joints;
    };
}    

#endif
