#ifndef ACES_HARDWARE_HPP
#define ACES_HARDWARE_HPP

#include <fstream>
#include <iostream>

#include <rtt/TaskContext.hpp>
#include <rtt/Event.hpp>
#include <rtt/Ports.hpp>
#include <rtt/Logger.hpp>
#include <rtt/Activity.hpp>

#include "message.hpp"
//#include "protocol.hpp"
#include "taskcfg.hpp"
#include "word.hpp"
#include "controller.hpp"

namespace ACES {
    //template <class HW, class P>
    //class Protocol;

    class ProtoHardware {
        public:
            ProtoHardware();
            virtual bool subscribeController(Controller* c);
            virtual bool rxBus();
    };

    template <class T>
    class Hardware : public ProtoHardware,
                     public RTT::TaskContext,
                     protected taskCfg
    {
        public:
            Hardware(std::string cfg, std::string args);
            //Hardware(std::string name);
            void updateHook();

            bool rxDownStream(Message<T>* m);

            virtual bool txBus(Message<T>* m);

            virtual Word<T>* processUSQueue();
            virtual Message<T>* processDSQueue();

        protected:
            RTT::Event<void( Word<T>* )> txUpStream;
            //T, read, write
            RTT::Queue< Word<T>*, RTT::NonBlockingPolicy,
                       RTT::BlockingPolicy> usQueue;
            RTT::Queue< Message<T>*, RTT::NonBlockingPolicy,
                       RTT::BlockingPolicy> dsQueue;
    };
}

#include "hardware.cc"
#endif
