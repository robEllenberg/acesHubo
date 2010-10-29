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

    template <class T>
    class Hardware;
//! Abstract class for describing a data protocol
/*!
 * The Protocol virtual class 
 */
    template <class T>
    class Protocol : protected taskCfg, 
                     public RTT::TaskContext
    {
        public:
            Protocol(std::string cfg, std::string args);
            void updateHook();

            virtual void rxDownStream(Goal*);
            virtual void rxUpStream(Word<T>);

            virtual Message<T>* processDSQueue();
            //Goal* getDSQelement();
            //std::deque<Goal*> dsQueue;
            //RTT::OS::Mutex dsqGuard; 

            virtual ProtoResult* processUSQueue();
            //Word<T> getUSQelement();
            //std::deque< Word<T> > usQueue;
            //RTT::OS::Mutex usqGuard; 

            bool subscribeDevice(Device* d);
        protected:
            RTT::Event<void( Message<T>* )> txDownStream;
            RTT::Event<void(ProtoResult*)> txUpStream;
            //T, read, write
            RTT::Queue< Word<T>, RTT::BlockingPolicy,
                       RTT::BlockingPolicy> usQueue;
            RTT::Queue<Goal*, RTT::NonBlockingPolicy,
                       RTT::BlockingPolicy> dsQueue;
    };

}
#endif
