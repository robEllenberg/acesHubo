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

    bool Credentials::operator==(const Credentials& other){
        if(devID == other.devID){
            return true;
        }
        else{
            return false;
        }
    }

    void Credentials::printme(){
        RTT::Logger::log() << "(Base) Credentials: devID=" << devID << RTT::endlog();
    }
}
