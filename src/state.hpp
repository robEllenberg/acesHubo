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

#include "credentials.hpp"
#include "controller.hpp"
#include "taskcfg.hpp"
#include "goal.hpp"

namespace ACES {
    //Webots Component Types
    enum COMP_TYPE { JOINT=1 };

    class ProtoState : protected taskCfg,
                       public RTT::TaskContext
    {
        public:
            ProtoState(std::string config, std::string args) ;

            bool configureHook();
            bool startHook();
            void updateHook();  
            void stopHook();
            void cleanupHook();

            int propID;

            RTT::Event<void(Goal*)> announceGoal;
            bool subscribeController(WbController* c);
            void setGoal(std::map<std::string, void*>*);
    };

    template <class T>
    class State : public ProtoState {
        public:
            State(std::string config, std::string args);

            void printme();
            T value;
    };

    #include "state.cc"
}
    
#endif
