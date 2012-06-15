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

#include <string>
#include <map>

#include <rtt/TaskContext.hpp>
#include <rtt/InputPort.hpp>
#include <rtt/OutputPort.hpp>
#include <rtt/Activity.hpp>
#include <rtt/Logger.hpp>
#include <rtt/os/TimeService.hpp>

#include "../taskcfg.hpp"
#include "../word.hpp"
#include "matlab.hpp"

namespace ACES {
    class ProtoState : public ACESTask
    {
        public:
            ProtoState(std::string config, int nID, bool sampling) ;
            /// Connect the ProtoState with a Controller node.
            bool subscribeController(RTT::TaskContext* c);
            const std::string& getName();
            virtual std::string logVal() = 0;

            /// The identifying marker for an individual state. 
            int nodeID;
            /// Attribute to enable or disable sampling 
            bool samplingAttr;
            /*! \brief Attribute to enable or disable automatic calculation of
             * the integral of the quantity. */
            bool intEnable;
            /*! \brief Attribute to enable or disable automatic calculation of
             * the derivative of the quantity. */
            bool diffEnable;
            /*! \brief A threshold on the time between samples, used to lessen
             *  the impact of noise on the derivative. */
            float diffThreshold;

    };

    /*! \brief A snapshot of a State's value at a specific time instant.
     * 
     * The point of the sample blah blah blarg.
     *
     */
    template <class T>
    class Sample {
        public:
            Sample(); ///< Creates an unpopulated Sample object 

            ///Creates a populated Sample object
            Sample( 
                T val,                           ///< Value of the sample
                RTT::os::TimeService::ticks time /**< Time at which the sample
                                                      was taken, in ticks. */
            );

            /// Time associated with Sample.
            float getSec(); 
            /**<  \returns The time at which the sample was taken in
             *             seconds since program start. */

            /// Indicate the validity of a sample data structure.
            bool isValid();
            /**< \returns true if the sample is valid (populated) or false if
                          it is not valid (unpopulated). */

            T getVal();
            RTT::os::TimeService::ticks getTick();

        private:
            bool valid; ///< Indicates Sample validity
            T value; ///< Value associated with the Sample
            RTT::os::TimeService::ticks t; /**< Time at which the sample was
                                                taken */
    };

    template <class T>
    class History {
        public:
            History(
                int size,    /**< Size of the history. The number of data
                                  points to track. */

                Sample<T> ic /**< Initial condition. The first point in
                                  the history is pre-populated with the IC
                                  value. */
            ); 

            History(
                int size     /**< Size of the history. The number of data
                                  points to track. */
            );

            ///Get a Sample object from the history based on its index.
            Sample<T> getSample(
                int sampleNum /**< The number of the sample to
                                   retrieve. Index 0 is the most recent,
                                   historySize is the oldest available. */

            ); /**< \returns       The sample associated with index
                                   sampleNum. */

            /** \brief Get a Sample object from the History based on the
             *         time at which it was acquired (Unimplemented).*/
            Sample<T> getSampleSec(
                float sec       ///< Age of sample to search for in seconds.
              
            );/**< \returns          The Sample associated most closely with
                                     time sec */

            /** \brief Get a Sample object from the History based on the
             *         time at which it was acquired (Unimplemented). */
            Sample<T> getSampleTicks(
                RTT::os::TimeService::ticks t   /**< Age of sample to search
                                                 *   for in number of system
                                                 *   ticks. */

            );/**< \returns                     The Sample associated
                                                most closely with time t*/

            /*! \brief Add a sample to the history based on the current time
             *         and given value.
             *  \param value Value of the sample to store in the history.
             */
            void update(T value);

            void printme();
        private:
            /// Number of elements to be stored in the History
            int size; 
            /// The index of the last valid entry in the History, -1 for none
            int lastValid;
            /// Data structure for storing the history
            std::deque< Sample<T> > hist;
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
            /// The State'
            History<T> hist;
            /// Data port for sending data to a Device
            RTT::OutputPort< Word<T>* > txDownStream;
            /// Data port for receiving data from a Device
            RTT::InputPort< Word<T>* > rxUpStream;
            /// Data port for receiving data from a Controller
            RTT::InputPort< std::map<std::string, void*>* > rxDownStream;
            
            unsigned int port;
            MatlabIO<T> matio;
            RTT::Activity matActivity;
    };
}
    
#include "state.cc"

#endif
