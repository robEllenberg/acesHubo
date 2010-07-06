#include "parameter.hpp"
namespace ACES {
    Parameter::Parameter(std::string n, Credentials* c,
      int pri, int UpdateFreq)
      : RTT::TaskContext(n),
        sendGoal("sendGoal")
    {
        name = n;
        credentials = c;
        frequency = UpdateFreq;
        priority = pri;

        this->events()->addEvent(&sendGoal, "sendGoal", "credentials",
            "credentials associated w/the goal");

        this->setActivity(
            new RTT::Activity( priority, 1.0/UpdateFreq, 0, n )
        );
    }
   
    void Parameter::printme(){
        RTT::Logger::log() << "Parameter: " << this->name
        << ", (F:" << this->frequency << ", P:"
        << this->priority << ")" << std::endl;

        this->credentials->printme();
    }
    
    bool Parameter::configureHook(){
        return true;
    }
    
    bool Parameter::startHook(){
        return true;
    }
    
    void Parameter::updateHook(){
        sendGoal(this->credentials);
        //RTT::Logger::log() << "Update Parameter "
        //<< this->name << RTT::Logger::endl;

    }
    
    void Parameter::stopHook(){
    }
    
    void Parameter::cleanupHook(){
    }

    void Parameter::setGoal(std::map<std::string, ACES::PValue*>* p){
        std::map<std::string, ACES::PValue* >::iterator mypair;
        mypair = p->find( name );
        if(mypair != p->end() ){
            ACES::PValue* val = (*mypair).second;
            ACES::Credentials* c = (ACES::Credentials*) credentials->credCopy(val);
            //this->outport->Push(c);
            //printme();
            sendGoal(c);
        }
    }
}
