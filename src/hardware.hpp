#ifndef ACES_HARDWARE_HPP
#define ACES_HARDWARE_HPP

#include <fstream>
#include <iostream>

#include <rtt/TaskContext.hpp>
#include <rtt/Event.hpp>
#include <rtt/Ports.hpp>
#include <rtt/Logger.hpp>
#include <rtt/Activity.hpp>

#include "message.hpp"
#include "protocol.hpp"
#include "taskcfg.hpp"
#include "word.hpp"

namespace ACES {
    class Protocol;
    class Hardware : //protected taskCfg,
                     public RTT::TaskContext {
        public:
            //Hardware(taskCfg cfg, std::string args);
            Hardware(std::string name);
            bool configureHook();
            bool startHook();
            void stopHook();
            virtual void updateHook();
            void cleanupHook();
            virtual bool isBusy(void) = 0;

            virtual bool transmit(Message* m) = 0;
            virtual bool recieve() = 0;

            bool subscribeProtocol(Protocol* p);
            //RTT::Method<bool(void)> isBusyMethod;
            RTT::Event<void(ProtoWord*)> announceRx;
    };

/*
    class charDevHardware : public Hardware {
        public:
            //! Ouput from this Hardware to associated Protocol
            RTT::WriteBufferPort<ACES::Message*> outBuffer; 
            //! Input to this hardware from Protocol
            RTT::ReadBufferPort<ACES::Message*> inBuffer; 
            void updateHook();
            void checkForLineData();
            void checkForProtocolData();
            std::ifstream* hardpoint_in;
            std::ofstream* hardpoint_out;
            charDevHardware(std::string name,
                     std::ifstream *in,
                     std::ofstream *out, int priority,
                     int UpdateFreq);
    };
*/

}    
#endif
