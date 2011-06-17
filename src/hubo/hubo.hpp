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

#ifndef ACES_HUBO_HPP
#define ACES_HUBO_HPP

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

#include "huboCanDS.hpp"
#include "can4linux.h"  //Include the can4linux data structures

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

namespace Hubo{
    class CANHardware : public ACES::Hardware<canmsg_t*>
    {
        public:
            CANHardware(std::string cfg, std::string args);
            virtual bool txBus(ACES::Message<canmsg_t*>* m);
            virtual void rxBus(int size=0);
            bool startHook();
            void stopHook();
            //virtual bool processUS(ACES::Word<canmsg_t*>* w);
            bool genPacket(int ID, int len, 
                           std::string lower, std::string upper);
        protected:
            std::string fd;
            int rate;
            int channel;    //! File descriptor for the CAN access node
            RTT::OutputPort< ACES::Message<canmsg_t*>* > txDSLoop;
    };

    class Protocol : public ACES::Protocol<canmsg_t*, canMsg> {
        public:
            Protocol(std::string cfg, std::string args);
            virtual ACES::Message<canmsg_t*>*
                      processDS(ACES::Word<canMsg>*);
    };

    class Device : public ACES::Device<float, canMsg>{
        public:
            Device(std::string cfg, std::string args);
            virtual ACES::Word<HuboVia>* processDS(ACES::Word<float>*);
            //Set Gain (Position)
            //Set Gain (Torque)
            //Set/Send Encoder Zero (?)
            //Go Home
            //PWM Command?
            //Go Limit Pos?
    };
};

#endif
