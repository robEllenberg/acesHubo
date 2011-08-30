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

#ifndef ACES_DEVICE_HPP
#define ACES_DEVICE_HPP

#include <rtt/TaskContext.hpp>
//#include <rtt/Operation.hpp>
#include <rtt/Activity.hpp>
#include <rtt/base/PortInterface.hpp>
#include <rtt/OutputPort.hpp>
#include <rtt/InputPort.hpp>

#include "taskcfg.hpp"
#include "credentials.hpp"
//#include "state/state.hpp"
#include "word.hpp"

namespace ACES{
    class ProtoDevice : public ACESTask
    {
        public:
            ProtoDevice(std::string config);
            bool subscribeState(RTT::TaskContext* s);
        protected:
            bool DSlockout;
            bool USlockout;
    };

    template <class S, class PD>
    class Device : public ProtoDevice 
    {
        public:
            Device(std::string config);
            //Device(std::string name);
            //virtual void rxUpStream(Word<PD>*);
            //virtual void rxDownStream(Word<S>*);

            void updateHook();

            virtual Word<PD>* processDS(Word<S>*);
            virtual Word<S>* processUS(Word<PD>*);
            bool subscribeState(RTT::TaskContext* s);
            void printCred();

        protected:
            //RTT::Event<void(Word<PD>*)> txDownStream;
            //RTT::Event<void(Word<S>*)> txUpStream;
            //RTT::Queue< Word<S>*, RTT::BlockingPolicy,
            //           RTT::BlockingPolicy> usQueue;
            //RTT::Queue< Word<S>*, RTT::NonBlockingPolicy,
            //           RTT::BlockingPolicy> dsQueue;

            //RTT::Method<bool()> credMethod;
            RTT::InputPort< Word<S>* > rxDownStream;
            RTT::InputPort< Word<PD>* > rxUpStream;
            RTT::OutputPort< Word<PD>* > txDownStream;
            RTT::OutputPort< Word<S>* > txUpStream;

            Credentials* credentials;
    };
}

#include "device.cc"

#endif
