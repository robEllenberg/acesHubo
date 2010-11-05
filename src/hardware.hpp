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
#include "protocol.hpp"
#include "taskcfg.hpp"
#include "word.hpp"

namespace ACES {
    template <class HW, class P>
    class Protocol;

    template <class T>
    class Hardware : protected taskCfg,
                     public RTT::TaskContext {
        public:
            Hardware(std::string cfg, std::string args);
            //Hardware(std::string name);
            void updateHook();

            bool rxDownStream(Message<T>* m);

            virtual bool txBus(Message<T>* m);
            virtual bool rxBus();

            virtual HWord<T>* processUSQueue();
            virtual Message<T>* processDSQueue();

            virtual bool subscribeController(Controller* c) = 0;
        protected:
            RTT::Event<void( HWord<T>* )> txUpStream;
            //T, read, write
            RTT::Queue< HWord<T>*, RTT::NonBlockingPolicy,
                       RTT::BlockingPolicy> usQueue;
            RTT::Queue< Message<T>*, RTT::NonBlockingPolicy,
                       RTT::BlockingPolicy> dsQueue;
    };
}    

#include "hardware.cc"
#endif
