#ifndef ACES_TASKCFG_HPP
#define ACES_TASKCFG_HPP

#include <string>
#include <sstream>

namespace ACES{

    class taskCfg {
        protected:
            std::string name; 
            double freq;
            int priority;
        public:
            taskCfg(std::string cfg);
            std::string Name();
    };
}
#endif
