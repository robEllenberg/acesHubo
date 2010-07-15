#ifndef ACES_CONTROLLER_HPP
#define ACES_CONTROLLER_HPP

#include <vector>
#include <map>
#include <list>
#include <iostream>
#include <fstream>
#include <string>

#include <rtt/TaskContext.hpp>
#include <rtt/Event.hpp>
#include <rtt/Method.hpp>
#include <rtt/Activity.hpp>

//#include "state.hpp"
//#include "hardware.hpp"
#include "taskcfg.hpp"

enum WB_ENUM { WB_CTRL_HALT, WB_CTRL_RUN, WB_CTRL_STEP };

namespace ACES{
    //class Hardware;
    class WbController : public RTT::TaskContext {
        public:
            WbController(std::string n,
              const char* scriptFile, 
              int pri, int UpdateFreq);
            WbController(taskCfg cfg, std::string args);

            void updateHook();
            bool configureHook();
            bool startHook();
            void stopHook();
            void cleanupHook();

            void step();
            void run();
            void halt();
            std::map<std::string, void*>*
                getStateVector(bool echo=0);

            //std::map<std::string, State*> pmap;

            std::ifstream walkScript;

            std::map<std::string, void*>* stateVect;

            RTT::Event<void(std::map<std::string, void*>*)>
                applyStateVector;

            RTT::Method<void(void)> stepMethod;
            RTT::Method<void(void)> runMethod;
            RTT::Method<void(void)> haltMethod;
            RTT::Method<bool(std::string)> openScriptMethod;

            bool openScript(std::string scriptPath);

            std::string name;
            int frequency;
            int priority;
            //std::list<Hardware*> hwlist;
            //std::list<Protocol*> pcollist;
            int simState;
    };
}

#endif
