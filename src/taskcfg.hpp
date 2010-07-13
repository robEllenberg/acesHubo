#ifndef ACES_TASKCFG_HPP
#define ACES_TASKCFG_HPP

#include <string>
#include <sstream>

namespace ACES{

    class taskCfg {
        public:
            taskCfg(std::string cfg);
            std::string name; 
            double freq;
            int priority;
    };
}
#endif
