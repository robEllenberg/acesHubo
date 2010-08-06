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
            //'int junk' is simply here to disambiguate the function calls,
            //it does nothing 
            Device(std::string config, std::string junk);
            //Device(std::string name);
            RTT::Event<void(Goal*)> txDownStream;
            RTT::Event<void(ProtoResult*)> txUpStream;
            virtual void rxUpStream(ProtoResult* rx);
            virtual void rxDownStream(Goal* g);

            void updateHook();

            virtual Goal* processDSQueue();
            std::deque<Goal*> dsQueue;
            RTT::OS::Mutex dsqGuard;

            virtual ProtoResult* processUSQueue();
            std::deque<ProtoResult*> usQueue;
            RTT::OS::Mutex usqGuard;

            //void attachCredentials(ACES::Credentials* c);
            bool subscribeState(ProtoState* s);
            Credentials* credentials;
    };
}

#endif
