#ifndef ACES_CONTROLLER_HPP
#define ACES_CONTROLLER_HPP

#include <string>

#include <rtt/TaskContext.hpp>

#include "protocol.hpp"
#include "controller.hpp"
#include "state.hpp"

namespace ACES{
    class Dispatcher : public RTT::TaskContext {
        public:
            Dispatcher();
            addHardware(std::string cfg, int type, std::string args);
            addProtocol(std::string cfg, int type, std::string args);
            addState(std::string cfg, int type, std::string args);
            addController(std::string cfg, int type, std::string args);

            std::list<void*> stateList;
            std::list<Protocol*> pList;
            std::list<WbController*> cList;
            
            bool configureHook();
            bool startHook();
            void updateHook();
            void stopHook();
            void cleanupHook();
            
    };

    class taskCfg {
        public:
            taskCfg(std::sting cfg);
            std::string name; 
            double freq;
            int priority;
    };
}

#endif
