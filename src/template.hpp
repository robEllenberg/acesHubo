#ifndef ACES_TEMPLATE_HPP
#define ACES_TEMPLATE_HPP

#include "hardware.hpp"
#include "protocol.hpp"
#include "device.hpp"
//#include "state.hpp" //If you need to derrive the state (try to avoid this)
#include "credentials.hpp"
//#include "word.hpp" // Word/Result
//#include "goal.hpp"
//#include "message.hpp"

/* If you need to implement your own logging functions. (you shouldn't) */
//#include "logger.hpp" 

namespace Template{

    class Hardware : public ACES::Hardware {
        public:
            Hardware(std::string cfg, std::string args);
            virtual bool txBus(Message* m);
            virtual bool rxBus();
            virtual Message* processDSQueue();
            virtual ProtoWord* processUSQueue();
            virtual bool subscribeController(ACES::Controller* c);
    };
 
    class Protocol : public ACES::Protocol{
        public:
            Protocol(std::string cfg, std::string args);
            virtual void rxDownStream(Goal*);
            virtual void rxUpStream(ProtoWord*);
            virtual Message* processDSQueue();
            virtual ProtoResult* processUSQueue();
    };
   
    class Device : public ACES::Device {
        public:
            Device(std::string cfg, std::string args);
            virtual void rxUpStream(ProtoResult* rx);
            virtual void rxDownStream(Goal* g);
            virtual Goal* processDSQueue();
            virtual ProtoResult* processUSQueue();
    };
    
}



#endif
