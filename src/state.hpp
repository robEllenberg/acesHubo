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
#include "credentials.hpp"
#include "controller.hpp"
#include "taskcfg.hpp"

namespace ACES {
    //class WbController;
    template <class T>
    class State : public RTT::TaskContext {
        public:
            State(std::string n, Credentials* c,
                      int pri, int UpdateFreq, T val);
            State(taskCfg cfg, Credentials* c, T ic);

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
            T value;

            void setGoal(std::map<std::string, void*>*);
            //TODO - Not the best method of data passing
            RTT::Event<void(void*)> announceGoal;
            bool subscribeController(WbController* c);
            //virtual Credentials* parseDispArgs(std::string type,
            //                                   std::string args) = 0;
    };
    #include "state.cc"
}
    
#endif
