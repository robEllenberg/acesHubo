#ifndef ACES_DISPATCHER_HPP
#define ACES_DISPATCHER_HPP

#include <map>
#include <rtt/TaskContext.hpp>
#include <rtt/Ports.hpp>
#include <rtt/Activity.hpp>
#include <rtt/Logger.hpp>
#include <rtt/Event.hpp>
#include <rtt/Buffer.hpp>
#include "parameter.hpp"
#include "protocol.hpp"

namespace ACES {
    class Parameter;
    class Protocol;
    class Dispatcher : public RTT::TaskContext {
        public:
            //Parameter(std::string name, Protocol* pro);
            Dispatcher(std::string n, int pri, int UpdateFreq );
            bool configureHook();
            bool startHook();
            void updateHook(); 
            void stopHook();
            void cleanupHook();
            bool registerParameter(Parameter* pram, Protocol* pcol);
            //bool activate();
            //bool deactivate();
            std::string name;
            int frequency;
            int priority;
            
        private:
            //Protocol *gov;
            std::map<Parameter*, Protocol*> routeTable;
    };
}

#endif
