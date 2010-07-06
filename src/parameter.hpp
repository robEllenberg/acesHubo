#ifndef ACES_PARAMETER_HPP
#define ACES_PARAMETER_HPP

#include <iostream>

#include <rtt/TaskContext.hpp>
#include <rtt/Event.hpp>
#include <rtt/Ports.hpp>
#include <rtt/Activity.hpp>
#include <rtt/Logger.hpp>
#include <rtt/Method.hpp>
#include <rtt/Command.hpp>

//#include "protocol.hpp"
#include "dispatcher.hpp"
#include "credentials.hpp"

namespace ACES {
    class Dispatcher;
    class Parameter : public RTT::TaskContext {
        public:
            Parameter(std::string n, Credentials* c,
                      int pri, int UpdateFreq);

            bool configureHook();
            bool startHook();
            void updateHook();  
            void stopHook();
            void cleanupHook();

            void printme();

            std::string name;
            int frequency;
            int priority;
            Credentials* credentials;

            void setGoal(std::map<std::string, ACES::PValue*>*);
            RTT::Event<void(ACES::Credentials*)> sendGoal;
    };
}
    
#endif
