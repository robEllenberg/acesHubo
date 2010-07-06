#ifndef ACES_CONTROLLER_HPP
#define ACES_CONTROLLER_HPP

#include <vector>
#include <map>
#include <iostream>
#include <string>

#include <rtt/TaskContext.hpp>
#include <rtt/Event.hpp>

#include "parameter.hpp"
#include "pvalue.hpp"

enum WB_ENUM { WB_CTRL_HALT, WB_CTRL_RUN, WB_CTRL_STEP };

namespace ACES{

    class WbController : public RTT::TaskContext {
        public:
            WbController(std::string n,
              std::vector<ACES::Parameter*> pl, 
              const char* scriptFile, ACES::Hardware* hw_one,
              ACES::Protocol* pcol,
              int pri, int UpdateFreq);

            void updateHook();
            bool configureHook();
            bool startHook();
            void stopHook();
            void cleanupHook();

            void step();
            void run();
            void halt();
            std::map<std::string, ACES::PValue*>*
                getStateVector(bool echo=0);

            std::vector<ACES::Parameter*> plist;

            std::map<std::string, ACES::Parameter*> pmap;

            std::ifstream walkScript;

            std::map<std::string, ACES::PValue*>* stateVect;

            RTT::Event<void(std::map<std::string, ACES::PValue*>*)>
                applyStateVector;
            RTT::Method<void(void)> stepMethod;
            RTT::Method<void(void)> runMethod;
            RTT::Method<void(void)> haltMethod;
            RTT::Method<bool(std::string)> openScriptMethod;

            bool openScript(std::string scriptPath);

            std::string name;
            int frequency;
            int priority;
            std::list<ACES::Hardware*> hwlist;
            std::list<Protocol*> pcollist;
            ACES::Dispatcher* dispatcher;
            int simState;
    };
}

#endif
