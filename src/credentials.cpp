#include "credentials.hpp"
namespace ACES {
    ProtoCredential::ProtoCredential(int t){
        credType = t;
    }

    Credentials::Credentials(int type, int id_num)
        : ProtoCredential(type)
    {
        this->id = id_num;
    }

    Credentials::Credentials(Credentials *c)
        : ProtoCredential(c->credType)
    {
        this->id = c->id;
    }

    void Credentials::printme() {
        RTT::Logger::log() << "Credential: Type= " << credType
                           << ", ID= " << id;
        RTT::Logger::log() << RTT::endlog();
    }

    /*
    Credentials::~Credentials(){
        delete this->val;
    }
    */

/*
    void Credentials::printme(){
        //int* p = (int*)val;
        RTT::Logger::log() << "Credential w/id= " << this->id
            << " and value= ";
        
        if(p){
            this->val->printme();
        }
        else{
            RTT::Logger::log()  << "NULL";
        }
        
        RTT::Logger::log() << val << RTT::endlog();
    }
*/

/*
    Credentials* Credentials::credCopy(SValue* p){
        Credentials *c = new Credentials();
        return c;
    }
*/
}
