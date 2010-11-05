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
#include <rtt/Queue.hpp>
#include <rtt/Buffer.hpp>
#include <rtt/BufferPolicy.hpp>

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
            std::string getName();
            virtual std::string logVal() = 0;
            RTT::Attribute<int> nodeID;
            RTT::Attribute<bool> samplingAttr;
 
            //RTT::OS::Mutex dsqGuard;
            //RTT::Buffer< Goal* > *dsQueue;
            //std::deque<Goal*> dsQueue;
            //std::deque<ProtoResult*> usQueue;
            //RTT::OS::Mutex usqGuard;

            //TODO - Remove nodeID entirely in favor of nodeIDAttr
            //int nodeID;
    };


    template <class T>
    class State : public ProtoState
    {
        public:
            State(std::string config, int nID);
            void (*asgnfunct)(Word<T>*, void*);

            virtual void updateHook();  
            virtual void sample();
            virtual std::string logVal();

            RTT::Event<void(Word<T>*)> txDownStream;
            void rxDownStream(std::map<std::string, void*>*);
            void rxUpStream(Word<T>*);
            void printme();
            void go(T sp);
            //static void assign(ProtoResult* meas, void* me);
            static void assign(Word<T>* w, void* me);

            RTT::Attribute<T> value;
            RTT::Method<void(T)> goMethod;
            RTT::Method<void()> sampleMethod;

            bool subscribeController(Controller* c);

        protected:
            RTT::Queue< Word<T>*, RTT::BlockingPolicy,
                       RTT::BlockingPolicy> usQueue;
            RTT::Queue< Word<T>*, RTT::NonBlockingPolicy,
                       RTT::BlockingPolicy> dsQueue;
    };
}
    
#include "state.cc"

#endif
