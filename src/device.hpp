#ifndef ACES_DEVICE_HPP
#define ACES_DEVICE_HPP

#include <rtt/TaskContext.hpp>
#include <rtt/Event.hpp>
#include <rtt/Buffer.hpp>

#include "goal.hpp"
#include "taskcfg.hpp"
#include "credentials.hpp"
#include "state.hpp"
#include "word.hpp"

namespace ACES{
    template <class S, class PD>
    class Device :  protected taskCfg,
                    public RTT::TaskContext
    {
        public:
            Device(std::string config);
            //Device(std::string name);
            virtual void rxUpStream(Word<PD>*);
            virtual void rxDownStream(Word<S>*);

            void updateHook();

            virtual Word<PD>* processDSQueue();
            virtual Word<S>* processUSQueue();
            bool subscribeState(RTT::TaskContext* s);
            bool printCred();

        protected:
            //RTT::Event<void(Word<PD>*)> txDownStream;
            //RTT::Event<void(Word<S>*)> txUpStream;
            RTT::Method<bool()> credMethod;

            RTT::Queue< Word<S>*, RTT::BlockingPolicy,
                       RTT::BlockingPolicy> usQueue;
            RTT::Queue< Word<S>*, RTT::NonBlockingPolicy,
                       RTT::BlockingPolicy> dsQueue;

            Credentials* credentials;
    };
}

#include "device.cc"

#endif
