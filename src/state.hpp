#ifndef ACES_STATE_HPP
#define ACES_STATE_HPP

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

/*
    class ProtoState : public taskCfg,
                       public RTT::TaskContext
    {
        public:
            ProtoState(std::string config, int nID) ;


            //RTT::OS::Mutex dsqGuard;
            //RTT::Buffer< Goal* > *dsQueue;
            //std::deque<Goal*> dsQueue;
            //std::deque<ProtoResult*> usQueue;
            //RTT::OS::Mutex usqGuard;

            //TODO - Remove nodeID entirely in favor of nodeIDAttr
            //int nodeID;
    };
*/

    template <class T>
    class State : public taskCfg,
                  public RTT::TaskContext
    {
        public:
            State(std::string config, int nID);
            void (*asgnfunct)(SWord<T>, void*);

            virtual void updateHook();  
            virtual void sample();

            RTT::Event<void(SWord<T>)> txDownStream;
            void rxDownStream(std::map<std::string, void*>*);
            void rxUpStream(SWord<T>);
            void printme();
            void go(T sp);
            //static void assign(ProtoResult* meas, void* me);
            static void assign(SWord<T> w, void* me);

            RTT::Attribute<T> value;
            RTT::Method<void(T)> goMethod;
            RTT::Attribute<int> nodeID;
            RTT::Attribute<bool> samplingAttr;
            RTT::Method<void()> sampleMethod;

            bool subscribeController(Controller* c);

        protected:
            RTT::Queue< SWord<T>, RTT::BlockingPolicy,
                       RTT::BlockingPolicy> usQueue;
            RTT::Queue< SWord<T>, RTT::NonBlockingPolicy,
                       RTT::BlockingPolicy> dsQueue;
    };

    #include "state.cc"
}
    
#endif
