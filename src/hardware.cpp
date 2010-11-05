#include "hardware.hpp"
namespace ACES {
    ProtoHardware::ProtoHardware(std::string cfg, std::string args) :
        taskCfg(cfg),
        RTT::TaskContext(name),
        txUpStream("txUpStream"),
        dsQueue(10),
        usQueue(10)
    {
        this->events()->addEvent(&txUpStream, "txUpStream", "word",
                                 "Recieved Data");

        this->setActivity( new RTT::Activity( priority, 1.0/freq, 0,
                                              name));
    }


}
