#ifndef ACES_GOAL_HPP
#define ACES_GOAL_HPP

#include "credentials.hpp"

namespace ACES{
    enum MODES { REFRESH=1, SET};
    class Goal {
        public:
            Goal(int id, int mode, void* d=NULL);
            void printme();

            int nodeID; //!Identify the type of node on the Device
            int mode; //!The objective of this Goal packet (Refresh, Set, etc)
            void* data; //!Information used downstream e.g. Setpoint

            //!Filled in by Device to identify request to the protocol
            Credentials* cred; 
    };
}
#endif
