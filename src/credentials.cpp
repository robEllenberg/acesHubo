#include "credentials.hpp"
namespace ACES {
    ProtoCredential::ProtoCredential(int t){
        credType = t;
    }

    template <class T> 
    Credentials<T>::Credentials(int id_num, T v, int type)
        : ProtoCredential(type){
        this->id = id_num;
        this->val = v;
    }

    template <class T> 
    Credentials<T>::Credentials(Credentials *c, T v)
    {
        this->id = c->id;
        this->val = v;
        this->credType = c->credType;
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
