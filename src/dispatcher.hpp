#ifndef ACES_DISPATCHER_HPP
#define ACES_DISPATCHER_HPP

#define TESTSUITE 1

#include <string>

#include <rtt/TaskContext.hpp>
//#include <ocl/ReportingComponent.hpp>
//#include <ocl/FileReporting.hpp>

#include "protocol.hpp"
#include "controller.hpp"
#include "state.hpp"
#include "hardware.hpp"
#include "taskcfg.hpp"
#include "device.hpp"
#include "logger.hpp"
#ifdef WEBOTS
    #include "webots.hpp"
#endif
#ifdef TESTSUITE
    #include "testsuite.hpp"
#endif


namespace ACES{
    class Logger;
    class Dispatcher : public RTT::TaskContext {
        public:
            Dispatcher(std::string name);
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
            bool addLogger(std::string cfg, std::string type,
                           std::string args);
            bool linkPD(std::string pcol, std::string device);
            bool linkDS(std::string device, std::string state);
            bool linkHP(std::string hw, std::string pcol);
            bool linkSC(std::string state, std::string ctrl);
            bool linkHC(std::string hw, std::string ctrl);
            //bool linkLS(std::string logger, std::string state);

            RTT::Method<bool(std::string, std::string, std::string)>
                        addHardwareMethod;
            RTT::Method<bool(std::string, std::string, std::string)>
                        addProtocolMethod;            
            RTT::Method<bool(std::string, std::string, std::string)>
                        addStateMethod;            
            RTT::Method<bool(std::string, std::string, std::string)>
                        addControllerMethod;            
            RTT::Method<bool(std::string, std::string, std::string)>
                        addDeviceMethod;            
            RTT::Method<bool(std::string, std::string, std::string)>
                        addLoggerMethod;            
                                                                                                
            std::list<ProtoState*> stateList;
            std::list<Protocol*> pList;
            std::list<Controller*> cList;
            std::list<Device*> dList;
            std::list<Hardware*> hwList;
            std::list<Logger*> logList;
            
            bool configureHook();
            bool startHook();
            void updateHook();
            void stopHook();
            void cleanupHook();
    };
}

#endif
