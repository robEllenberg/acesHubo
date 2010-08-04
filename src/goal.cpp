#include "goal.hpp"

namespace ACES{

    Goal::Goal(int id, int m, void* d){
        propID = id;
        data = d;
        mode = m;
        cred = 0;
    }

    void Goal::printme(){
        //RTT::Logger::log() << "Goal (";
        switch(mode){
            case (REFRESH):
                //RTT::Logger::log() << "Refresh";
                cred->printme();     
                break;
            case (SET):
                //RTT::Logger::log() << "Set";
                break;
            default:
                break;
        }
        //RTT::Logger::log() << "): PID=" << propID;
        //TODO - Implement a function to actually display the data.
        if(data){
            //RTT::Logger::log() << " w/Data";
        }
        else{
            //RTT::Logger::log() << " w/NoData ";
        }

        if(! cred){
            //RTT::Logger::log() << " NoCredentials";
            //RTT::Logger::log() <<RTT::endlog();
        }
        else{
            //RTT::Logger::log() <<RTT::endlog();
        }
    }

}
