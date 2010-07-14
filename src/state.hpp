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
    class ProtoState : public RTT::TaskContext {
        public:
            ProtoState(std::string n, Credentials* c,
            int pri, int UpdateFreq);
            ProtoState(taskCfg cfg, Credentials* c) ;
            bool configureHook();
            bool startHook();
            void updateHook();  
            void stopHook();
            void cleanupHook();

            std::string name;
            int frequency;
            int priority;
            Credentials* credentials;

            RTT::Event<void(Credentials*)> announceGoal;
            bool subscribeController(WbController* c);
            void setGoal(std::map<std::string, void*>*);
    };

    template <class T>
    class State : public ProtoState {
        public:
            State(std::string n, Credentials* c,
                      int pri, int UpdateFreq, T val);
            State(taskCfg cfg, Credentials* c, T ic);

            void printme();
            T value;

            //TODO - Not the best method of data passing
            //virtual Credentials* parseDispArgs(std::string type,
            //                                   std::string args) = 0;
    };
    #include "state.cc"
}
    
#endif
