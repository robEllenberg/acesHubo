#ifndef ACES_CONTROLLER_HPP
#define ACES_CONTROLLER_HPP

#include <string>

#include <rtt/TaskContext.hpp>

#include "protocol.hpp"
#include "controller.hpp"
#include "state.hpp"
#include "hardware.hpp"
#include "webots.hpp"

namespace ACES{
    class Dispatcher : public RTT::TaskContext {
        public:
            Dispatcher();
            bool addHardware(std::string cfg, std::string type,
                             std::string args);
            bool addProtocol(std::string cfg, std::string type,
                             std::string args);
            bool addState(std::string cfg, std::string type,
                          std::string args);
            bool addController(std::string cfg, std::string type,
                               std::string args);

            std::list<void*> stateList;
            std::list<Protocol*> pList;
            std::list<WbController*> cList;
            std::list<Hardware*> hwList;
            
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
