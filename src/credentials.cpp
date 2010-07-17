#include "credentials.hpp"
namespace ACES {
    ProtoCredential::ProtoCredential(int t){
        credType = t;
    }

    Credentials::Credentials(int type)
        : ProtoCredential(type)
    {
    }

    Credentials::Credentials(Credentials *c)
        : ProtoCredential(c->credType)
    {
    }

    void Credentials::printme() {
        RTT::Logger::log() << "Credential: Type= " << credType;
        RTT::Logger::log() << RTT::endlog();
    }

}
