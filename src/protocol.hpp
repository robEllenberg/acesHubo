#ifndef ACES_PROTOCOL_HPP
#define ACES_PROTOCOL_HPP

#include <vector>
#include <iostream>
#include <string>

#include <rtt/TaskContext.hpp>
#include <rtt/base/PortInterface.hpp>
#include <rtt/InputPort.hpp>
#include <rtt/OutputPort.hpp>
#include <rtt/Logger.hpp>
#include <rtt/Activity.hpp>

#include "message.hpp"
#include "state.hpp"
#include "taskcfg.hpp"
#include "device.hpp"
#include "word.hpp"

namespace ACES {
    class Credentials;

    template <class T>
    class State;

    template <class H>
    class Hardware;

    class ProtoProtocol : protected taskCfg, 
                          public RTT::TaskContext
    {
        public:   
            ProtoProtocol(std::string cfg, std::string args);
            bool subscribeDevice(RTT::TaskContext* d);
    };

//! Abstract class for describing a data protocol
/*!
 * The Protocol virtual class 
 */
    template <class HW, class PD>    
    class Protocol : public ProtoProtocol
    {
        public:
            Protocol(std::string cfg, std::string args);
            void updateHook();

            //virtual void rxDownStream(Word<PD>*);
            //virtual void rxUpStream(Word<HW>*);

            virtual Message<HW>* processDS(Word<PD>*);
            virtual Word<PD>* processUS(Word<HW>*);

        protected:
            //RTT::Event<void(Message<HW>*)> txDownStream;
            //RTT::Event< void(Word<PD>*) > txUpStream;
            //T, read, write
            RTT::InputPort< Word<PD>* > rxDownStream;
            RTT::InputPort< Word<HW>* > rxUpStream;
            RTT::OutputPort< Message<HW>* > txDownStream;
            RTT::OutputPort< Word<PD>* > txUpStream;
            /*
            RTT::Queue< Word<HW>*, RTT::NonBlockingPolicy,
                       RTT::BlockingPolicy> usQueue;
            RTT::Queue<Word<PD>*, RTT::NonBlockingPolicy,
                       RTT::BlockingPolicy> dsQueue;
            */
    };
}

#include "protocol.cc"
#endif
