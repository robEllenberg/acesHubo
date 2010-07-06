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
#include "svalue.hpp"

namespace ACES {
      
    class Hardware : public RTT::TaskContext {
        public:
          //! Ouput from this Hardware to associated Protocol
            RTT::WriteBufferPort<ACES::Message*> outBuffer; 
            //! Input to this hardware from Protocol
            RTT::ReadBufferPort<ACES::Message*> inBuffer; 
            int frequency;
            int priority;

            Hardware(std::string name, int priority,
                     int UpdateFreq);
            bool configureHook();
            bool startHook();
            void stopHook();
            virtual void updateHook();
            void cleanupHook();
            virtual bool isBusy(void) = 0;

            virtual bool transmit(Message* m) = 0;
            virtual bool recieve() = 0;

            //RTT::Event<void(std::string)> NewData;
            RTT::Method<bool(void)> isBusyMethod;
    };

    class charDevHardware : public Hardware {
        public:
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

}    
#endif
