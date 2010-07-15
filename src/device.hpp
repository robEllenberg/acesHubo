#ifndef ACES_DEVICE_HPP
#define ACES_DEVICE_HPP

#include <rtt/TaskContext.hpp>
#include <rtt/Event.hpp>

#include "goal.hpp"
#include "taskcfg.hpp"
#include "credentials.hpp"

namespace ACES{
    class Device : public RTT::TaskContext {
        public:
            Device(taskCfg cfg, Credentials* c);
            void RxGoal(Goal* g);
            bool subscribeState(RTT::TaskContext* s);

            RTT::Event<void(Goal*)> TxRequest;
            Credentials* credentials;
    };
}

#endif
