#include "credentials.hpp"
namespace ACES {

    //Credentials::Credentials(){
    //    this->id = 0;
    //    this->val = 0;
    //}
    Credentials::Credentials(int id_num, ACES::PValue* p){
        this->id = id_num;
        this->val = p;
    }

    Credentials::Credentials(Credentials *c, ACES::PValue* p)
    {
        this->id = c->id;
        this->val = p;
    }

    Credentials::~Credentials(){
        delete this->val;
    }

    void Credentials::printme(){
        int* p = (int*)val;
        RTT::Logger::log() << "Credential w/id= " << this->id
            << " and value= ";
        if(p){
            this->val->printme();
        }
        else{
            RTT::Logger::log()  << "NULL";
        }
        RTT::Logger::log() << RTT::endlog();
    }


/*
    Credentials* Credentials::credCopy(PValue* p){
        Credentials *c = new Credentials();
        return c;
    }
*/
}
