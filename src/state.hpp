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
#include <rtt/os/TimeService.hpp>

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
            bool intEnable;
            bool diffEnable;
            bool subscribeController(RTT::TaskContext* c);
    };

    template <class T>
    class Sample {
        public:
            Sample();
            Sample(T val, RTT::os::TimeService::ticks time);
            T getVal();
            RTT::os::TimeService::ticks getTick();
            float getSec();
            bool isValid();
        private:
            bool valid;
            T value;
            RTT::os::TimeService::ticks t;
    };

    template <class T>
    class History {
        public:
            History(int size, Sample<T> ic);
            History(int size);
            Sample<T> getSample(int sampleNum);
            Sample<T> getSampleSec(float sec);
            Sample<T> getSampleTicks(RTT::os::TimeService::ticks t);
            void update(T value);
        private:
            int size;
            int lastValid;
            std::deque< Sample<T> > hist;
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
            double getInt();
            double getDiff();

            Word<T>* processDS( std::map<std::string, void*>* p );

        protected:
            T value;
            double integral;
            double diff;
            History<T> hist;
            RTT::OutputPort< Word<T>* > txDownStream;
            RTT::InputPort< Word<T>* > rxUpStream;
            RTT::InputPort< std::map<std::string, void*>* > rxDownStream;
    };

}
    
#include "state.cc"

#endif
