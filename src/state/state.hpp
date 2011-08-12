/* 
    Conductor: High Degree of Freedom Robot Controller Framework
    Copyright (C) 2010, 2011 Robert Sherbert
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
    please contact the author. The software can be licensed to you under
    non-free license terms by the copyright holder.

    As a courtesy to the author, and in the spirit of fair attribution, you are
    asked to cite the following paper if any work based upon or utilizing this
    framework is published in the scientific literature: 
    Sherbert, Robert M. and Oh, Paul Y. "Conductor: A Controller Development
    Framework for High Degree of Freedom Systems." Intelligent Robots and
    Systems (IROS), 2011 IEEE/RSJ International Conference on. 
*/

#ifndef ACES_STATE_HPP
#define ACES_STATE_HPP

//#include <iostream>
#include <string>
#include <map>

//#include <boost/bind.hpp>

#include <rtt/TaskContext.hpp>
#include <rtt/InputPort.hpp>
#include <rtt/OutputPort.hpp>
#include <rtt/Activity.hpp>
#include <rtt/Logger.hpp>
//#include <rtt/Operation.hpp>
//#include <rtt/Attribute.hpp>
#include <rtt/os/TimeService.hpp>

//#include "../credentials.hpp"
//#include "../controller.hpp"
#include "../taskcfg.hpp"
#include "../word.hpp"
#include "matlab.hpp"

#include <boost/circular_buffer.hpp>

namespace ACES {
    //Webots Component Types
    //enum DATA_TYPE { BOOL=1, SHORT, INT, LONG, FLOAT, DOUBLE, VPOINT };

    class ProtoState : public ACESTask
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
            const std::string& getName();
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
            boost::circular_buffer< Sample<T> > hist;
    };

    template <class T>
    class State : public ProtoState
    {
        public:
            State(std::string config, int nID, bool sampling, unsigned int
                  portnum);
            bool startHook();
            void stopHook();

            virtual void updateHook();  
            virtual void sample();
            virtual std::string logVal();
            void printme();
            void printHistory();
            virtual void go(T sp);
            virtual void assign(Word<T>* w);
            virtual T getVal();
            float getInt();
            float getDiff();

            bool updateInt(Sample<T> cur, Sample<T> last);
            bool updateDiff(Sample<T> cur, Sample<T> last);

            virtual Word<T>* processDS( std::map<std::string, void*>* p );

        protected:
            T value;
            float integral;
            float diff;
            float kInt;
            History<T> hist;
            RTT::OutputPort< Word<T>* > txDownStream;
            RTT::InputPort< Word<T>* > rxUpStream;
            RTT::InputPort< std::map<std::string, void*>* > rxDownStream;
            
            unsigned int port;
            MatlabIO<T> matio;
            RTT::Activity matActivity;
    };
    
    template <class T>
    class ProtoFilter
    {
        public:
            ProtoFilter();
            virtual bool update(T newVal)=0;
            virtual T getOutput();
        protected:
            T value;
    };

    template <class T>
    class SmoothingFilter : public ProtoFilter<T>
    {
        public:
            SmoothingFilter(int size=10);
            bool update(T newVal);
            bool setDelay(int samples);
        private:
            int sanitizeDelay(int samples);
            void trimBuffer(int count);
            std::deque<T> xBuffer;
            std::deque<T> vBuffer;
            std::deque<T> aBuffer;
            T oldValue;
            int size;
    };

    template <class T>
    class FastFilter : public ProtoFilter<T>
    {
        public:
            FastFilter(int size=10);
            bool update(T newVal);
            bool setDelay(int samples);
        private:
            boost::circular_buffer<T> xBuffer;
            boost::circular_buffer<T> vBuffer;
            boost::circular_buffer<T> aBuffer;
            T oldValue;
            int size;
    };

    template <class T>
    class DFFilter : public ProtoFilter<T>
    {
        public:
            /** Constructor to take coefficient vectors */
            DFFilter(std::vector<T> a,std::vector<T> b);
            bool update(T newVal);
        private:

            /**
             * xHist is a simple input data history
             * yHist is a history of outputs, used for the feedback portion
             * a is the coefficients of the denominator, b the numerator
             */
            boost::circular_buffer<T> xHist;
            boost::circular_buffer<T> yHist;
            std::vector<T> a;
            std::vector<T> b;
    };
    template <class T>
    class FilteredState : public State<T>
    {
        public:
            FilteredState(std::string config, int nID, bool sampling, unsigned int portnum, int fLength);
            //TODO: Make fLength unsigned

            void go(T sp);
            void assign(Word<T>* w);
            T getVal();
            void setDelay(int fLength);
        protected:
            FastFilter<float> filt;
    };
}
    
#include "state.cc"

#endif
