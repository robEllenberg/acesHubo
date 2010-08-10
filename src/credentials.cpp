#include "credentials.hpp"
namespace ACES {
    Credentials::Credentials(int id){
        devID = id;
    }

    Credentials::Credentials(std::string args){
        std::istringstream s1(args, std::istringstream::in);
        int idnum;
        s1 >> idnum;
        this->devID = idnum;
    }

    void Credentials::printme(){
        RTT::Logger::log() << "Credentials: ID=" << devID << RTT::endlog();
    }
}
