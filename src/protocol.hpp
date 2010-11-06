#ifndef ACES_PROTOCOL_HPP
#define ACES_PROTOCOL_HPP

#include <vector>
#include <iostream>
#include <string>

#include <rtt/TaskContext.hpp>
#include <rtt/Ports.hpp>
#include <rtt/PortInterface.hpp>
#include <rtt/Logger.hpp>
#include <rtt/Activity.hpp>
#include <rtt/Buffer.hpp>

#include "message.hpp"
#include "state.hpp"
#include "taskcfg.hpp"
#include "goal.hpp"
#include "device.hpp"
#include "word.hpp"

namespace ACES {
    class Credentials;

    template <class T>
    class State;

    template <class H>
    class Hardware;

    class ProtoProtocol
    {
        public:    
            ProtoProtocol();
    };

//! Abstract class for describing a data protocol
/*!
 * The Protocol virtual class 
 */
    template <class HW, class PD>    
    class Protocol : protected taskCfg, 
                     public RTT::TaskContext
    {
        public:
            Protocol(std::string cfg, std::string args);
            void updateHook();

            virtual void rxDownStream(Word<PD>*);
            virtual void rxUpStream(Word<HW>*);

            virtual Message<HW>* processDSQueue();
            virtual Word<PD>* processUSQueue();

            //'subscribe' used in the upstream direction, 'connect' used
            //for downstream direction
            template<class S>
            bool subscribeDevice(RTT::TaskContext* d);
            bool connectHardware(RTT::TaskContext* h);
        protected:
            RTT::Event<void(Message<HW>*)> txDownStream;
            RTT::Event< void(Word<PD>*) > txUpStream;
            //T, read, write
            RTT::Queue< Word<HW>*, RTT::NonBlockingPolicy,
                       RTT::BlockingPolicy> usQueue;
            RTT::Queue<Word<PD>*, RTT::NonBlockingPolicy,
                       RTT::BlockingPolicy> dsQueue;
    };
}

#include "protocol.cc"
#endif
