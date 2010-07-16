#ifndef ACES_DISPATCHER_HPP
#define ACES_DISPATCHER_HPP

#include <string>

#include <rtt/TaskContext.hpp>

#include "protocol.hpp"
#include "controller.hpp"
#include "state.hpp"
#include "hardware.hpp"
#include "webots.hpp"
#include "taskcfg.hpp"
#include "device.hpp"

namespace ACES{
    class Dispatcher : public RTT::TaskContext {
        public:
            Dispatcher(std::string n, int pri, int freq);
            bool addHardware(std::string cfg, std::string type,
                             std::string args);
            bool addProtocol(std::string cfg, std::string type,
                             std::string args);
            bool addState(std::string cfg, std::string type,
                          std::string args);
            bool addController(std::string cfg, std::string type,
                               std::string args);
            bool addDevice(std::string cfg, std::string type,
                           std::string args);
            bool linkPD(std::string pcol, std::string device);
            bool linkDS(std::string device, std::string state);
            bool linkHP(std::string hw, std::string pcol);
            bool linkSC(std::string state, std::string ctrl);
            bool linkHC(std::string hw, std::string ctrl);

            std::list<void*> stateList;
            std::list<Protocol*> pList;
            std::list<WbController*> cList;
            std::list<Device*> dList;
            std::list<Hardware*> hwList;
            
            bool configureHook();
            bool startHook();
            void updateHook();
            void stopHook();
            void cleanupHook();
            
    };
}

#endif
