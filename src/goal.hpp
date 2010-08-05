#ifndef ACES_GOAL_HPP
#define ACES_GOAL_HPP

#include "credentials.hpp"

namespace ACES{
    enum MODES { REFRESH=1, SET};
    class Goal {
        public:
            Goal(int id, int mode, void* d=0);
            void printme();

            int nodeID;
            int mode;
            void* data;
            Credentials* cred;
    };
}
#endif
