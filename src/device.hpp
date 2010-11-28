#ifndef ACES_DEVICE_HPP
#define ACES_DEVICE_HPP

#include <rtt/TaskContext.hpp>
#include <rtt/Operation.hpp>
#include <rtt/base/PortInterface.hpp>
#include <rtt/OutputPort.hpp>
#include <rtt/InputPort.hpp>

#include "taskcfg.hpp"
#include "credentials.hpp"
#include "state.hpp"
#include "word.hpp"

namespace ACES{
    class ProtoDevice : protected taskCfg,
                        public RTT::TaskContext 
    {
        public:
            ProtoDevice(std::string config);
            bool subscribeState(RTT::TaskContext* s);
    };

    template <class S, class PD>
    class Device : public ProtoDevice 
    {
        public:
            Device(std::string config);
            //Device(std::string name);
            //virtual void rxUpStream(Word<PD>*);
            //virtual void rxDownStream(Word<S>*);

            void updateHook();

            virtual Word<PD>* processDS(Word<S>*);
            virtual Word<S>* processUS(Word<PD>*);
            bool subscribeState(RTT::TaskContext* s);
            void printCred();

        protected:
            //RTT::Event<void(Word<PD>*)> txDownStream;
            //RTT::Event<void(Word<S>*)> txUpStream;
            //RTT::Queue< Word<S>*, RTT::BlockingPolicy,
            //           RTT::BlockingPolicy> usQueue;
            //RTT::Queue< Word<S>*, RTT::NonBlockingPolicy,
            //           RTT::BlockingPolicy> dsQueue;

            //RTT::Method<bool()> credMethod;
            RTT::InputPort< Word<S>* > rxDownStream;
            RTT::InputPort< Word<PD>* > rxUpStream;
            RTT::OutputPort< Word<PD>* > txDownStream;
            RTT::OutputPort< Word<S>* > txUpStream;

            Credentials* credentials;
    };
}

#include "device.cc"

#endif
