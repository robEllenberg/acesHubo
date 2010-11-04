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
    template <class S, class P>
    class Device :  protected taskCfg,
                    public RTT::TaskContext
    {
        public:
            Device(std::string config);
            //Device(std::string name);
            virtual void rxUpStream(PDWord<P>*);
            virtual void rxDownStream(SWord<S>*);

            void updateHook();

            virtual SWord<S>* processDSQueue();
            virtual std::deque< PDWord<P>* > processUSQueue();
            //bool subscribeState(ProtoState* s);
            bool subscribeState(RTT::TaskContext* s);
            bool printCred();
/*
            Goal* getDSQelement();
            std::deque<Goal*> dsQueue;
            RTT::OS::Mutex dsqGuard;

            ProtoResult* getUSQelement();
            std::deque<ProtoResult*> usQueue;
            RTT::OS::Mutex usqGuard;
*/
        protected:
            RTT::Event<void(PDWord<P>*)> txDownStream;
            RTT::Event<void(SWord<S>*)> txUpStream;
            RTT::Method<bool()> credMethod;

            RTT::Queue< SWord<S>*, RTT::BlockingPolicy,
                       RTT::BlockingPolicy> usQueue;
            RTT::Queue< SWord<S>*, RTT::NonBlockingPolicy,
                       RTT::BlockingPolicy> dsQueue;

            Credentials* credentials;
    };
}

#endif
