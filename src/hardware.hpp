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

#ifndef ACES_HARDWARE_HPP
#define ACES_HARDWARE_HPP

#include <fstream>
//#include <iostream>

#include <boost/asio.hpp>

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
//#include "controller.hpp"

namespace ACES {
    //template <class HW, class P>
    //class Protocol;

    class ProtoHardware : public ACESTask
    {
        public:
            ProtoHardware(std::string cfg, std::string args);
            bool subscribeProtocol(RTT::TaskContext* p);
            virtual bool subscribeController(RTT::TaskContext* c);
            RTT::OutputPort<int> packetReporter;
    };

    template <class T>
    class Hardware : public ProtoHardware
    {
        public:
            Hardware(std::string cfg, std::string args);
            void updateHook();
            void reportTransmission(Message<T>*);

            virtual bool txBus(Message<T>* m);
            virtual void rxBus(int size=0);

            virtual bool processUS(Word<T>*);
            virtual bool processDS(Message<T>*);
            bool subscribeProtocol(RTT::TaskContext* p);

        protected:
            RTT::internal::Queue< Word<T>* > usQueue;
            RTT::OutputPort< Word<T>* > txUpStream;
            RTT::InputPort< Message<T>* > rxDownStream;
    };

    class charHardware : public Hardware<unsigned char>
    {
        public:
            charHardware(std::string cfg, std::string args);
            bool txBus(ACES::Message<unsigned char>* m);
        protected:
            boost::asio::io_service io_service;
            boost::asio::serial_port port;
    };

    class pStreamHardware : public Hardware<unsigned char>
    {
        public:
            pStreamHardware(std::string cfg, std::string args);
            bool txBus(ACES::Message<unsigned char>* m);
        protected:
            int ioFD;
            //boost::asio::io_service io_service;
            //boost::asio::posix::stream_descriptor port;
    };
}

#include "hardware.cc"
#endif
