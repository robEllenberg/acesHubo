/* AUTORIGHTS
    ACES: Abstracted Controls Engineering Software
    Copyright (C) 2010 Robert Sherbert
    bob.sherbert@gmail.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    If you are interested in licensing this software for commercial purposes
    please contact the author.
*/

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
#ifdef HUBO 
    #include "hubo/hubo.hpp"
#endif
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
                          std::string args, bool sampling);
            bool addController(std::string cfg, std::string type,
                               std::string args);
            bool addDevice(std::string cfg, std::string type,
                           std::string args);
            bool addLogger(std::string cfg, std::string type,
                           std::string args);

            bool startDebug();
            bool stopDebug();

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
            bool linkLS(std::string logger, std::string state,
                        std::string attr);

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
