#include "dispatcher.hpp"

namespace ACES{
    Dispatcher::Dispatcher(){}
    
    }

    addHardware(std::string cfg, int type, std::string args){
        taskCfg c(cfg);
        
        switch(type){
        }
    }

    addProtocol(std::string cfg, std::string type, std::string args){

    }

    addState(std::string cfg, std::string type, std::string args){

    }

    addController(std::string cfg, std::string type, std::string args){

    }

    taskCfg::taskCfg(std::string cfg){
        std::istringstream s1(cfg);
        s1 >> name;
        s1 >> pri;
        s1 >> freq;
    }
