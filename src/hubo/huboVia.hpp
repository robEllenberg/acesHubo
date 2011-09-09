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

#ifndef ACES_HUBOVIA_HPP
#define ACES_HUBOVIA_HPP

#include <sstream>
#include <iomanip>
#include <cmath>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/array.hpp>

#include "../hardware.hpp"
#include "../device.hpp"
#include "../protocol.hpp"
#include "../message.hpp"
#include "../word.hpp"
#include "../credentials.hpp"

#include "huboTxPacket.hpp"

using boost::asio::ip::udp;

namespace HuboVia {

    class HuboVia {
        public:
            HuboVia();
            HuboVia(double pos);
            double getRad();
            double getDeg();
        private:
           double rad;
    };

    class Credentials : public ACES::Credentials {
        public:
            Credentials(int id, double zero, int direction);
            double getZero();
            int getDirection();
            void printme();
        protected:
            double zero;
            int direction;
    };

    class Hardware : public ACES::Hardware<unsigned char*> {
        public:
            Hardware(std::string cfg, std::string args);
            bool startHook();
            void stopHook();
            //virtual bool processDS(Message<unsigned char>*);
            virtual bool txBus(ACES::Message<unsigned char*>* m);
        protected:
            boost::array<unsigned char, 8> send_buf;
            boost::asio::io_service io_service;
            udp::resolver resolver;
            udp::resolver::query query;
            udp::endpoint receiver_endpoint;
            udp::socket socket;
    };

    class Protocol : public ACES::Protocol<unsigned char*, HuboVia> {
        public:
            Protocol(std::string cfg, std::string args);
            virtual ACES::Message<unsigned char*>*
                      processDS(ACES::Word<HuboVia>*);
    };

    class Device : public ACES::Device<float, HuboVia>{
        public:
            Device(std::string cfg, std::string args);
            virtual ACES::Word<HuboVia>* processDS(ACES::Word<float>*);
    };
};
#endif
