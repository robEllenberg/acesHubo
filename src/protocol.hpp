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

#include "message.hpp"
#include "state.hpp"
#include "taskcfg.hpp"
#include "goal.hpp"
#include "device.hpp"

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
            bool configureHook();
            bool startHook();
            void updateHook();
            void stopHook();
            void cleanupHook();
            bool subscribeDevice(Device* d);

            //! Processed, waiting for transmission. 
            /*! The set of messages already processed by the
             protocol waiting to be
             submitted to the hardware for transmission.
             */
            std::deque<Message*>* pending_stack;
            //! The requests made to protocol by parameters
            //RTT::ReadBufferPort<Credentials*>* request_stack;

            //std::list<Credentials*>* getNewRequests();
            void addRequest(Goal*);

            //virtual void aggregateRequests(
            //    std::list<Credentials*> &reqs) = 0;

            //virtual Message* buildMessage(
            //                   Credentials* cred) = 0;

            //virtual Credentials<T>* parseHWInput(
            //                   Message* c) = 0 ;
            //void issueMessage();

            Message* prepareMessage();
            RTT::Event<void(Message*)> issueMessage;
            
            //virtual bool registerParam(ACES::State*) = 0;
    };

    class charDevProtocol : public Protocol {
        
    };
}    
#endif
