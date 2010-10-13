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
    class Device :  protected taskCfg,
                    public RTT::TaskContext
    {
        public:
            Device(std::string config);
            //Device(std::string name);
            RTT::Event<void(Goal*)> txDownStream;
            RTT::Event<void(ProtoResult*)> txUpStream;
            virtual void rxUpStream(ProtoResult* rx);
            virtual void rxDownStream(Goal* g);

            void updateHook();

            virtual Goal* processDSQueue();
            Goal* getDSQelement();
            std::deque<Goal*> dsQueue;
            RTT::OS::Mutex dsqGuard;

            virtual std::list<ProtoResult*> processUSQueue();
            ProtoResult* getUSQelement();
            std::deque<ProtoResult*> usQueue;
            RTT::OS::Mutex usqGuard;

            //void attachCredentials(ACES::Credentials* c);
            bool subscribeState(ProtoState* s);
            Credentials* credentials;

            bool printCred();
            RTT::Method<bool()> credMethod;
    };
}

#endif
