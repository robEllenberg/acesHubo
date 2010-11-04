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

    template <class H>
    class Hardware;
//! Abstract class for describing a data protocol
/*!
 * The Protocol virtual class 
 */
    template <class HW, class P>    //A hardware-word based class & protocol
                                    //specific one.
    class Protocol : protected taskCfg, 
                     public RTT::TaskContext
    {
        public:
            Protocol(std::string cfg, std::string args);
            void updateHook();

            virtual void rxDownStream(PDWord<P>);
            virtual void rxUpStream(HWord<HW>);

            virtual Message<HW>* processDSQueue();
            //Goal* getDSQelement();
            //std::deque<Goal*> dsQueue;
            //RTT::OS::Mutex dsqGuard; 

            virtual PDWord<P> processUSQueue();
            //Word<w> getUSQelement();
            //std::deque< Word<W> > usQueue;
            //RTT::OS::Mutex usqGuard; 

            template<class S>
            bool subscribeDevice(RTT::TaskContext* d);
            //'subscribe' used in the upstream direction, 'connect' used
            //for downstream direction
            bool connectHardware(RTT::TaskContext* h);
        protected:
            RTT::Event<void(Message<HW>)> txDownStream;
            RTT::Event< void(PDWord<P>) > txUpStream;
            //T, read, write
            RTT::Queue< HWord<HW>, RTT::BlockingPolicy,
                       RTT::BlockingPolicy> usQueue;
            RTT::Queue<PDWord<P>, RTT::NonBlockingPolicy,
                       RTT::BlockingPolicy> dsQueue;
    };
}

#include "protocol.cc"
#endif
