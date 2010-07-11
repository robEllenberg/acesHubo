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
#include "state.hpp"

namespace ACES {
    template <class T>
    class Credentials;
    template <class T>
    class State;
//! Abstract class for describing a data protocol
/*!
 * The Protocol virtual class 
 */
    class Protocol : public RTT::TaskContext {
        public:
            Protocol(std::string name, Hardware* hw,
                     int pri, int UpdateFreq);
            Protocol(taskCfg cfg, std::string args);
            bool configureHook();
            bool startHook();
            //void updateHook(
            //const std::vector<RTT::PortInterface*>&
            //   updatedPorts);
            void updateHook();
            void stopHook();
            void cleanupHook();
            bool addHW(Hardware* hw);

            RTT::ReadBufferPort<Message*>* hwInBuffer;
            RTT::WriteBufferPort<Message*>* hwOutBuffer;
            //Hardware* hardware;
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
            //RTT::ReadBufferPort<Credentials*>* request_stack;

            //std::list<Credentials*>* getNewRequests();
            template <class T>
            void addRequest(Credentials<T>*);

            //virtual void aggregateRequests(
            //    std::list<Credentials*> &reqs) = 0;

            //virtual Message* buildMessage(
            //                   Credentials* cred) = 0;

            //virtual Credentials<T>* parseHWInput(
            //                   Message* c) = 0 ;
            //void issueMessage();

            Message* prepareMessage();
            RTT::Event<void(Message*)> issueMessage;
            
            bool theresStillTime();
            //virtual bool registerParam(ACES::State*) = 0;
            template <class T>
            bool registerState(State<T>* p);
            std::list<void*> pramlist;
    };

    class charDevProtocol : public Protocol {
        
    };
}    
#endif
