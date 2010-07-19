#ifndef ACES_DEVICE_HPP
#define ACES_DEVICE_HPP

#include <rtt/TaskContext.hpp>
#include <rtt/Event.hpp>

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
            void RxGoal(Goal* g);
            bool subscribeState(ProtoState* s);

            bool configureHook();
            void updateHook();
            void cleanupHook();

            virtual void interpretResult(ProtoResult* rx) = 0 ;

            RTT::Event<void(Goal*)> TxRequest;
            RTT::Event<void(Goal*)> announceData;
            Credentials* credentials;
    };
}

#endif
