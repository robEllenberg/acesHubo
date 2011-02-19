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

#ifndef ACES_FLEXPARSER_HPP
#define ACES_FLEXPARSER_HPP

#include <rtt/base/RunnableInterface.hpp>
#include <rtt/OutputPort.hpp>

#include <boost/asio.hpp>

#include "../protocol.hpp"
#include "../message.hpp"
#include "../state.hpp"
#include "../hardware.hpp"
#include "../credentials.hpp"

namespace FlexScanner {
    class Reader : public RTT::base::RunnableInterface {
        public:
            Reader(boost::asio::serial_port* p);
            bool initialize();
            void step();
            void loop();
            void finalize();
        //private:
            unsigned char rxBuf;
            boost::asio::serial_port* port;
            RTT::OutputPort< unsigned char > out;
    };

    class Hardware : public ACES::Hardware<unsigned char>{
        public:
            Hardware(std::string cfg, std::string args);
            //virtual bool txBus(ACES::Message<unsigned char>* m);
            virtual void rxBus(int size=0);
            bool startHook();
            void stopHook();
        protected:
            boost::asio::io_service io_service;
            boost::asio::serial_port port;
            RTT::InputPort<unsigned char> rxBuf;
            Reader rxReader;
            RTT::Activity rxActivity;
    };

    template <class USPacket, class FlexScanner>
    class Protocol : public ACES::Protocol<unsigned char, USPacket> {
        public:
            Protocol(std::string cfg, std::string args); 
            bool startHook();
            void stopHook();
            ACES::Word<USPacket>* processUS(ACES::Word<unsigned char>* w);
            virtual ACES::Credentials* credFromPacket(USPacket* p) = 0;

            FlexScanner* scanner;
            unsigned char incoming_id;
            unsigned char incoming_len;
    };
}

#include "flexscanner.cc"
#endif
