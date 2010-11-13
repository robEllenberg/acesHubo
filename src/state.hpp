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
            virtual std::string logVal() = 0;
            int nodeID;
            bool samplingAttr;
            bool subscribeController(RTT::TaskContext* c);
    };

    template <class T>
    class State : public ProtoState
    {
        public:
            State(std::string config, int nID);

            virtual void updateHook();  
            virtual void sample();
            virtual std::string logVal();

            void printme();
            virtual void go(T sp);
            void assign(Word<T>* w);
            Word<T>* processDS( std::map<std::string, void*>* p );

            T value;

        protected:
            RTT::OutputPort< Word<T>* > txDownStream;
            RTT::InputPort< Word<T>* > rxUpStream;
            RTT::InputPort< std::map<std::string, void*>* > rxDownStream;
    };

    class DerrivedState : public State
}
    
#include "state.cc"

#endif
