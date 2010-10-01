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
#include "word.hpp"

namespace ACES {
    //Webots Component Types
    //enum DATA_TYPE { BOOL=1, SHORT, INT, LONG, FLOAT, DOUBLE, VPOINT };

    class ProtoState : public taskCfg,
                       public RTT::TaskContext
    {
        public:
            ProtoState(std::string config, int nID) ;
            void (*asgnfunct)(ProtoResult*, void*);

            virtual void updateHook();  
            void sample();

            RTT::Event<void(Goal*)> txDownStream;
            void rxDownStream(std::map<std::string, void*>*);
            void rxUpStream(ProtoResult* d);

            int nodeID;
            RTT::Buffer< Goal* > *set_stack;
            RTT::Method<void()> sampleMethod;

            bool subscribeController(Controller* c);
    };

    template <class T>
    class State : public ProtoState {
        public:
            State(std::string config, int nID);

            void printme();
            void go(T sp);
            static void assign(ProtoResult* meas, void* me);

            RTT::Attribute<T> value;
            RTT::Method<void(T)> goMethod;
    };

    #include "state.cc"
}
    
#endif
