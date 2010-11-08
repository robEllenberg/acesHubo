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
            virtual void rxBus();
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
