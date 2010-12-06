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

#ifndef ACES_HARDWARE_HPP
#define ACES_HARDWARE_HPP

#include <fstream>
#include <iostream>

#include <rtt/TaskContext.hpp>
//#include <rtt/Event.hpp>
#include <rtt/OutputPort.hpp>
#include <rtt/InputPort.hpp>
#include <rtt/Logger.hpp>
#include <rtt/Activity.hpp>
#include <rtt/internal/Queue.hpp>

#include "message.hpp"
//#include "protocol.hpp"
#include "taskcfg.hpp"
#include "word.hpp"
#include "controller.hpp"

namespace ACES {
    //template <class HW, class P>
    //class Protocol;

    class ProtoHardware : protected taskCfg,
                          public RTT::TaskContext
    {
        public:
            ProtoHardware(std::string cfg, std::string args);
            bool subscribeProtocol(RTT::TaskContext* p);
            virtual bool subscribeController(RTT::TaskContext* c);
    };

    template <class T>
    class Hardware : public ProtoHardware
    {
        public:
            Hardware(std::string cfg, std::string args);
            //Hardware(std::string name);
            void updateHook();

            //bool rxDownStream(Message<T>* m);

            virtual bool txBus(Message<T>* m);
            virtual void rxBus(int size=0);
            //void txUpStream( Word<T>* );

            virtual bool processUS(Word<T>*);
            virtual bool processDS(Message<T>*);
            bool subscribeProtocol(RTT::TaskContext* p);

        protected:
            //RTT::Event<void( Word<T>* )> txUpStream;
            //T, read, write
            RTT::internal::Queue< Word<T>* > usQueue;
            //, RTT::NonBlockingPolicy,
            //           RTT::BlockingPolicy> usQueue;
            /*
            RTT::Queue< Message<T>*, RTT::NonBlockingPolicy,
                       RTT::BlockingPolicy> dsQueue;
            */
            RTT::OutputPort< Word<T>* > txUpStream;
            RTT::InputPort< Message<T>* > rxDownStream;
    };
}

#include "hardware.cc"
#endif
