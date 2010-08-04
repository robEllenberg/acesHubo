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
            void rxDownStream(Goal* g);
            void attachCredentials(ACES::Credentials* c);
            bool subscribeState(ProtoState* s);

            bool configureHook();
            void updateHook();
            void cleanupHook();

            virtual void rxUpStream(ProtoResult* rx) = 0 ;

            RTT::Event<void(Goal*)> txDownStream;
            RTT::Event<void(Goal*)> txUpStream;
            Credentials* credentials;
            RTT::Buffer<Goal*> *requestBuf;
            RTT::Buffer<Goal*> *returnBuf;
    };
}

#endif
