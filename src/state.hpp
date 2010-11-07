#ifndef ACES_STATE_HPP
#define ACES_STATE_HPP

#include <iostream>
#include <string>
#include <map>

#include <rtt/TaskContext.hpp>
#include <rtt/InputPort.hpp>
#include <rtt/OutputPort.hpp>
#include <rtt/Activity.hpp>
#include <rtt/Logger.hpp>
#include <rtt/Operation.hpp>
#include <rtt/Attribute.hpp>

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
            //std::string getName();
            virtual std::string logVal() = 0;
            int nodeID;
            bool samplingAttr;
            //RTT::Attribute<int> nodeID;
            //RTT::Attribute<bool> samplingAttr;
 
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

            //RTT::Event<void(Word<T>*)> txDownStream;
            //void rxDownStream(std::map<std::string, void*>*);
            //void rxUpStream(Word<T>*);
            void printme();
            void go(T sp);
            //static void assign(ProtoResult* meas, void* me);
            static void assign(Word<T>* w, void* me);
            Word<T>* processDS( std::map<std::string, void*>* p );

            //RTT::Attribute<T> value;
            T value;
            //RTT::Method<void(T)> goMethod;
            //RTT::Method<void()> sampleMethod;

            //bool subscribeController(Controller* c);

        protected:
            /*RTT::Queue< Word<T>*, RTT::BlockingPolicy,
                       RTT::BlockingPolicy> usQueue;
            RTT::Queue< Word<T>*, RTT::NonBlockingPolicy,
                       RTT::BlockingPolicy> dsQueue;*/
            RTT::OutputPort< Word<T>* > txDownStream;
            RTT::InputPort< Word<T>* > rxUpStream;
            RTT::InputPort< std::map<std::string, void*>* > rxDownStream;
    };
}
    
#include "state.cc"

#endif
