#include "taskcfg.hpp"
namespace ACES{

    taskCfg::taskCfg(std::string cfg){
        std::istringstream s1(cfg, std::istringstream::in);
        s1 >> name;
        s1 >> priority;
        s1 >> freq;
    }

    std::string taskCfg::Name(){
        return name;
    }
}
