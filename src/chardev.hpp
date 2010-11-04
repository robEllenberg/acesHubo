#include "hardware.hpp"

namespace CharDev {
    class Hardware : public ACES::Hardware{
        public:
            Hardware(std::string cfg, std::string args);
            bool startHook();
            void stopHook();
            virtual bool txBus(Message* m);
            virtual bool rxBus();
            virtual Message* processDSQueue();
            virtual ProtoWord* processUSQueue();
    }
}
