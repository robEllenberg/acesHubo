#ifndef ACES_DISPATCHER_HPP
#define ACES_DISPATCHER_HPP

#include <map>
#include <rtt/TaskContext.hpp>
#include <rtt/Ports.hpp>
#include <rtt/Activity.hpp>
#include <rtt/Logger.hpp>
#include <rtt/Event.hpp>
#include <rtt/Buffer.hpp>
#include "state.hpp"
#include "protocol.hpp"

namespace ACES {
    class State;
    class Protocol;
    class Dispatcher : public RTT::TaskContext {
        public:
            //State(std::string name, Protocol* pro);
            Dispatcher(std::string n, int pri, int UpdateFreq );
            bool configureHook();
            bool startHook();
            void updateHook(); 
            void stopHook();
            void cleanupHook();
            bool registerState(State* pram, Protocol* pcol);
            //bool activate();
            //bool deactivate();
            std::string name;
            int frequency;
            int priority;
            
        private:
            //Protocol *gov;
            std::map<State*, Protocol*> routeTable;
    };
}

#endif
