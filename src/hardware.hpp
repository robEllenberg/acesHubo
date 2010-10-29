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
    template <class T>
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

            virtual Word<T> processUSQueue();
            //T, read, write
            //Word<T> getUSQelement();
            //std::deque< Word<T> > usQueue;
            //RTT::OS::Mutex usqGuard; 

            virtual Message<T>* processDSQueue();
            //Message<T>* getDSQelement();
            //std::deque< Message<T>* > dsQueue;
            //RTT::OS::Mutex dsqGuard; 

            bool subscribeProtocol(Protocol<T>* p);
            virtual bool subscribeController(Controller* c) = 0;
        protected:
            RTT::Event<void( Word<T> )> txUpStream;
            RTT::Queue< Word<T>, RTT::BlockingPolicy,
                       RTT::BlockingPolicy> usQueue;
            RTT::Queue< Message<T>*, RTT::NonBlockingPolicy,
                       RTT::BlockingPolicy> dsQueue;
    };
}    

#include "hardware.cc"
#endif
