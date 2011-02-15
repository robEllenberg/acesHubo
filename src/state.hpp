/* AUTORIGHTS
    ACES: Abstracted Controls Engineering Software
    Copyright (C) 2010 Robert Sherbert
    bob.sherbert@gmail.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    If you are interested in licensing this software for commercial purposes
    please contact the author.
*/

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
#include "word.hpp"

namespace ACES {
    //Webots Component Types
    //enum DATA_TYPE { BOOL=1, SHORT, INT, LONG, FLOAT, DOUBLE, VPOINT };

    class ProtoState : public taskCfg,
                       public RTT::TaskContext
    {
        public:
            ProtoState(std::string config, int nID, bool sampling) ;
            virtual std::string logVal() = 0;
            int nodeID;
            bool samplingAttr;
            bool intEnable;
            bool diffEnable;
            float diffThreshold;
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
            void printme();
        private:
            int size;
            int lastValid;
            std::deque< Sample<T> > hist;
    };

    template <class T>
    class State : public ProtoState
    {
        public:
            State(std::string config, int nID, bool sampling);

            virtual void updateHook();  
            virtual void sample();
            virtual std::string logVal();
            void printme();
            void printHistory();
            virtual void go(T sp);
            void assign(Word<T>* w);
            T getVal();
            float getInt();
            float getDiff();
            bool updateInt(Sample<T> cur, Sample<T> last);
            bool updateDiff(Sample<T> cur, Sample<T> last);

            Word<T>* processDS( std::map<std::string, void*>* p );

        protected:
            T value;
            float integral;
            float diff;
            History<T> hist;
            RTT::OutputPort< Word<T>* > txDownStream;
            RTT::InputPort< Word<T>* > rxUpStream;
            RTT::InputPort< std::map<std::string, void*>* > rxDownStream;

            boost::asio::io_service io_service;
            udp::socket socket;
            udp::endpoint sender_endpoint_;
            enum { max_length = 1024 };
            char data_[max_length];

    };

}
    
#include "state.cc"

#endif
