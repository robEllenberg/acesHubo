#include "parameter.hpp"
namespace ACES {
    Parameter::Parameter(std::string n, Credentials* c,
      Dispatcher* d, int pri, int UpdateFreq)
      : RTT::TaskContext(n)
    {
        name = n;
        credentials = c;
        dispatcher = d;
        frequency = UpdateFreq;
        priority = pri;

        setPoint = new RTT::Command<bool(ACES::PValue*)>
            ("setpoint", &Parameter::setGoal,
            &Parameter::goalSet, this);
        this->commands()->addCommand(
            setPoint, "ah", "sp", "The set point"
        );

        this->setActivity(
            new RTT::Activity( priority, 1.0/UpdateFreq )
        );
    }
    
    bool Parameter::triggerCycle(){
        return true;
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
        if (triggerCycle()){ 
            this->outport->Push(this->credentials);
        }
        //RTT::Logger::log() << "Update Parameter "
        //<< this->name << RTT::Logger::endl;

    }
    
    void Parameter::stopHook(){
    }
    
    void Parameter::cleanupHook(){
    }

    bool Parameter::setGoal(PValue* val){
        //TODO - See if this can be  removed in favor of
        //original approach 
        //Credentials* c = new Credentials(credentials, val);
        Credentials* c = credentials->credCopy(val);
        this->outport->Push(c);
        return true;
    }

    bool Parameter::goalSet(){
        return true;
    }

}
