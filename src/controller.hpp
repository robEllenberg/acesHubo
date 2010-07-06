#ifndef ACES_CONTROLLER_HPP
#define ACES_CONTROLLER_HPP

#include <vector>
#include <map>
#include <iostream>
#include <string>

#include <rtt/TaskContext.hpp>
#include <rtt/Event.hpp>

#include "state.hpp"
#include "pvalue.hpp"

enum WB_ENUM { WB_CTRL_HALT, WB_CTRL_RUN, WB_CTRL_STEP };

namespace ACES{

    class WbController : public RTT::TaskContext {
        public:
            WbController(std::string n,
              std::vector<State*> pl, 
              const char* scriptFile, Hardware* hw_one,
              Protocol* pcol,
              int pri, int UpdateFreq);

            void updateHook();
            bool configureHook();
            bool startHook();
            void stopHook();
            void cleanupHook();

            void step();
            void run();
            void halt();
            std::map<std::string, PValue*>*
                getStateVector(bool echo=0);

            std::vector<State*> plist;

            std::map<std::string, State*> pmap;

            std::ifstream walkScript;

            std::map<std::string, PValue*>* stateVect;

            RTT::Event<void(std::map<std::string, PValue*>*)>
                applyStateVector;
            RTT::Method<void(void)> stepMethod;
            RTT::Method<void(void)> runMethod;
            RTT::Method<void(void)> haltMethod;
            RTT::Method<bool(std::string)> openScriptMethod;

            bool openScript(std::string scriptPath);

            std::string name;
            int frequency;
            int priority;
            std::list<Hardware*> hwlist;
            std::list<Protocol*> pcollist;
            int simState;
    };
}

#endif
