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

#include "taskcfg.hpp"

enum WB_CTRL_STATES { WB_CTRL_HALT, WB_CTRL_RUN, WB_CTRL_STEP };

namespace ACES{
    class Controller : protected taskCfg,
                       public RTT::TaskContext
    {
        public:
            Controller(std::string cfg, std::string args);
            bool configureHook();
            bool startHook();
            void stopHook();
            void cleanupHook();
    };

    class ScriptCtrl : public Controller
    {
        public:
            ScriptCtrl(std::string cfg, std::string args);
            void updateHook();

            void step();
            void run();
            void halt();
            bool openScript(std::string scriptPath);
            virtual std::map<std::string, void*>* 
                    getStateVector(bool echo=0) = 0;

            std::ifstream walkScript;
            std::map<std::string, void*>* stateVect;
            int simState;

            RTT::Method<void(void)> stepMethod;
            RTT::Method<void(void)> runMethod;
            RTT::Method<void(void)> haltMethod;
            RTT::Method<bool(std::string)> openScriptMethod;

            RTT::Event<void(std::map<std::string, void*>*)>
                applyStateVector;
    };

}

#endif
