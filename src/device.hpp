#ifndef ACES_DEVICE_HPP
#define ACES_DEVICE_HPP

#include <rtt/TaskContext.hpp>
#include <rtt/Event.hpp>

#include "goal.hpp"
#include "taskcfg.hpp"
#include "credentials.hpp"
#include "state.hpp"

namespace ACES{
    class Device :  protected taskCfg,
                    public RTT::TaskContext
    {
        public:
            Device(std::string config);
            void RxGoal(Goal* g);
            bool subscribeState(ProtoState* s);

            bool configureHook();
            bool startHook();
            void updateHook();
            void stopHook();
            void cleanupHook();

            RTT::Event<void(Goal*)> TxRequest;
            Credentials* credentials;
    };
}

#endif
