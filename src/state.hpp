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
    class History {
        public:
            History(int size, Sample<T> ic);
            History(int size);
            Sample<T> getSample(int sampleNum);
            Sample<T> getSampleSec(float sec);
            Sample<T> getSampleTicks(RTT::os::TimeSerivce::ticks t);
        private:
            int size;
            int lastValid;
            std::vector< Sample<T> > hist;
    };

    template <class T>
    class Sample {
        public:
            T getVal();
            RTT::os::TimeService::ticks getTick();
            float getSec();
        private:
            T value;
            RTT::os::TimeService::ticks t;
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
            T getVal();

            Word<T>* processDS( std::map<std::string, void*>* p );

        protected:
            T value;
            History<T> hist;
            RTT::OutputPort< Word<T>* > txDownStream;
            RTT::InputPort< Word<T>* > rxUpStream;
            RTT::InputPort< std::map<std::string, void*>* > rxDownStream;
    };

    class DerrivedState : public State
}
    
#include "state.cc"

#endif
