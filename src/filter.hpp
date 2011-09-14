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

#ifndef ACES_FILTER_HPP
#define ACES_FILTER_HPP

//#include <iostream>
#include <string>
#include <map>

//#include <boost/bind.hpp>

#include <rtt/TaskContext.hpp>
#include <rtt/InputPort.hpp>
#include <rtt/OutputPort.hpp>
#include <rtt/Activity.hpp>
#include <rtt/Logger.hpp>
#include <rtt/os/TimeService.hpp>

#include "taskcfg.hpp"
#include "word.hpp"
#include "state/state.hpp"

#include <boost/circular_buffer.hpp>

namespace ACES {

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
    
#include "filter.cc"

#endif
