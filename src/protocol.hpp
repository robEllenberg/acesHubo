#ifndef ACES_PROTOCOL_HPP
#define ACES_PROTOCOL_HPP

#include <rtt/TaskContext.hpp>
#include <rtt/Ports.hpp>
#include <rtt/PortInterface.hpp>
#include <rtt/Logger.hpp>
#include <rtt/Activity.hpp>
#include <vector>
#include <iostream>
#include <string>
#include "message.hpp"
#include "hardware.hpp"
#include "parameter.hpp"
//#include "webots.hpp"

//namespace Webots{
//    class Webots;
//}

namespace ACES {
    class Dispatcher;
    class Credentials;
//! Abstract class for describing a data protocol
/*!
 * The Protocol virtual class 
 */
    class Protocol : public RTT::TaskContext {
        public:
            Protocol(std::string name, Hardware* hw,
                     int pri, int UpdateFreq);
            bool configureHook();
            bool startHook();
            //void updateHook(
            //const std::vector<RTT::PortInterface*>&
            //   updatedPorts);
            void updateHook();
            void stopHook();
            void cleanupHook();

            RTT::ReadBufferPort<Message*>* hwInBuffer;
            RTT::WriteBufferPort<Message*>* hwOutBuffer;
            Hardware* hardware;
            std::string name;
            int frequency;
            int priority;
            //! Processed, waiting for transmission. 
            /*! The set of messages already processed by the
             protocol waiting to be
             submitted to the hardware for transmission.
             */
            std::deque<Message*>* pending_stack;
            //! The requests made to protocol by parameters
            RTT::ReadBufferPort<Credentials*>* request_stack;

            std::list<Credentials*>* getNewRequests();
            virtual void aggregateRequests(
                std::list<Credentials*> &reqs) = 0;

            //virtual Message* buildMessage(
            //                   Credentials* cred) = 0;

            virtual Credentials* parseHWInput(
                               Message* c) = 0 ;
            void issueMessage();
            
            bool theresStillTime();
            //virtual bool registerParam(ACES::Parameter*) = 0;
    };

    class charDevProtocol : public Protocol {
        
    };
}    
#endif
