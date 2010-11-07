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
#ifdef ROBOTIS 
    #include "robotis.hpp"
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

            bool startHW();
            bool startProtocol();
            bool startDevice();
            bool startState();
            bool startController();
            bool startLogger();

            bool stopHW();
            bool stopProtocol();
            bool stopDevice();
            bool stopState();
            bool stopController();
            bool stopLogger();

            bool linkPD(std::string pcol, std::string device);
            bool linkDS(std::string device, std::string state);
            bool linkHP(std::string hw, std::string pcol);
            bool linkSC(std::string state, std::string ctrl);
            bool linkHC(std::string hw, std::string ctrl);
            bool linkLS(std::string logger, std::string state);

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

            RTT::Method<bool(void)> startHWMethod;
            RTT::Method<bool(void)> startProtocolMethod;
            RTT::Method<bool(void)> startDeviceMethod;
            RTT::Method<bool(void)> startStateMethod;
            RTT::Method<bool(void)> startControllerMethod;
            RTT::Method<bool(void)> startLoggerMethod;

            RTT::Method<bool(void)> stopHWMethod;
            RTT::Method<bool(void)> stopProtocolMethod;
            RTT::Method<bool(void)> stopDeviceMethod;
            RTT::Method<bool(void)> stopStateMethod;
            RTT::Method<bool(void)> stopControllerMethod;
            RTT::Method<bool(void)> stopLoggerMethod;

            RTT::Method<bool(std::string, std::string)> linkPDMethod;
            RTT::Method<bool(std::string, std::string)> linkDSMethod;
            RTT::Method<bool(std::string, std::string)> linkHPMethod;
            RTT::Method<bool(std::string, std::string)> linkSCMethod;
            RTT::Method<bool(std::string, std::string)> linkHCMethod;
            RTT::Method<bool(std::string, std::string)> linkLSMethod;

            std::list<RTT::TaskContext*> stateList;
            std::list<RTT::TaskContext*> pList;
            std::list<RTT::TaskContext*> cList;
            std::list<RTT::TaskContext*> dList;
            std::list<RTT::TaskContext*> hwList;
            std::list<RTT::TaskContext*> logList;
            
            bool startHook();
            void stopHook();
    };
}

#endif
