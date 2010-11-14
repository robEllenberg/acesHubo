#ifndef ACES_HUBO_HPP
#define ACES_HUBO_HPP

#include <sstream>
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

namespace Hubo {

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
