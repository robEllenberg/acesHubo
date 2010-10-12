#ifndef ACES_PROTOCOL_HPP
#define ACES_PROTOCOL_HPP

#include <vector>
#include <iostream>
#include <string>

#include <rtt/TaskContext.hpp>
#include <rtt/Ports.hpp>
#include <rtt/PortInterface.hpp>
#include <rtt/Logger.hpp>
#include <rtt/Activity.hpp>
#include <rtt/Buffer.hpp>

#include "message.hpp"
#include "state.hpp"
#include "taskcfg.hpp"
#include "goal.hpp"
#include "device.hpp"
#include "word.hpp"

namespace ACES {
    class Credentials;
    template <class T>
    class State;
    class Hardware;
//! Abstract class for describing a data protocol
/*!
 * The Protocol virtual class 
 */
    class Protocol : protected taskCfg, 
                     public RTT::TaskContext
    {
        public:
            Protocol(std::string cfg, std::string args);
            //Protocol(std::string name);
            void updateHook();

            RTT::Event<void(Message*)> txDownStream;
            RTT::Event<void(ProtoResult*)> txUpStream;
            virtual void rxDownStream(Goal*);
            virtual void rxUpStream(ProtoWord*);

            virtual Message* processDSQueue();
            std::deque<Goal*> dsQueue;
            RTT::OS::Mutex dsqGuard; 

            virtual ProtoResult* processUSQueue();
            std::deque<ProtoWord*> usQueue;
            RTT::OS::Mutex usqGuard; 

            bool subscribeDevice(Device* d);
    };
}    
#endif
