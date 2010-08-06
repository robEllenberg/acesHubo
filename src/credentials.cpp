#include "credentials.hpp"
namespace ACES {
    Credentials::Credentials(int id){
        devID = id;
    }

    void Credentials::printme(){
        RTT::Logger::log() << "Credentials: ID=" << devID << RTT::endlog();
    }
}
