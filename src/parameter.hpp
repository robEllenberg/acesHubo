#ifndef ACES_PARAMETER_HPP
#define ACES_PARAMETER_HPP

#include <iostream>

#include <rtt/TaskContext.hpp>
#include <rtt/Event.hpp>
#include <rtt/Ports.hpp>
#include <rtt/Activity.hpp>
#include <rtt/Logger.hpp>
#include <rtt/Method.hpp>
#include <rtt/Command.hpp>

//#include "protocol.hpp"
#include "dispatcher.hpp"
#include "credentials.hpp"

namespace ACES {
    class Dispatcher;
    class Parameter : public RTT::TaskContext {
        public:
            std::string name;
            int frequency;
            int priority;

            Credentials* credentials;
            Dispatcher* dispatcher;
            /*!
                The outport is created and linked to the 
                paramter by the dispatcher as part of the
                connection process.
            */
            RTT::WriteBufferPort<Credentials*>* outport;
            RTT::ReadDataPort<Credentials*>* inport;
            //RTT::Buffer<Credentials*, RTT::NonBlockingPolicy,
            //            RTT::BlockingPolicy>* outport;
            //RTT::Buffer<Credentials*>* inport;

            //RTT::Method<void(float)> setMethod;

            Parameter(std::string n, Credentials* c,
                      Dispatcher* d, int pri, int UpdateFreq);

            bool configureHook();
            bool startHook();
            void updateHook();  
            void stopHook();
            void cleanupHook();

            bool triggerCycle();
            void printme();

            virtual bool setGoal(ACES::PValue*) ;
            virtual bool goalSet() ;
            RTT::Command<bool(ACES::PValue*)>* setPoint;
    };
}
    
#endif
