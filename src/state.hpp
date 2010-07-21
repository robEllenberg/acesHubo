#ifndef ACES_PARAMETER_HPP
#define ACES_PARAMETER_HPP

#include <iostream>
#include <string>

#include <rtt/TaskContext.hpp>
#include <rtt/Event.hpp>
#include <rtt/Ports.hpp>
#include <rtt/Activity.hpp>
#include <rtt/Logger.hpp>
#include <rtt/Method.hpp>
#include <rtt/Command.hpp>
#include <rtt/Buffer.hpp>

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

            void (*asgnfunct)(void*, void*);

            RTT::Event<void(Goal*)> announceGoal;
            RTT::Buffer< Goal* > *set_stack;
            bool subscribeController(Controller* c);
            void setGoal(std::map<std::string, void*>*);
            void RxData(Goal* d);
    };

    template <class T>
    class State : public ProtoState {
        public:
            State(std::string config, std::string args);

            void printme();
            void go(T sp);
            static void assign(void* meas, void* me);

            //T value;
            RTT::Attribute<T> value;
            RTT::Method<void(T)> goMethod;
    };

    #include "state.cc"
}
    
#endif
