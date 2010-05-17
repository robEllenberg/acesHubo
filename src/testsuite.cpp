#include "testsuite.hpp"

namespace TestSuite{

    Hardware::Hardware(std::string name, int priority,
             int UpdateFreq)
    : ACES::Hardware(name, priority, UpdateFreq){}

    bool Hardware::isBusy(){
        return false;
    }

    bool Hardware::transmit(ACES::Message* m){
        m->printme();
        //RTT::Logger::log(RTT::Logger::Debug) << "Update"
        //  << RTT::endlog();
    }

    bool Hardware::recieve(){
        return true;
    }

    Protocol::Protocol(std::string name, ACES::Hardware* hw,
                     int pri, int UpdateFreq)
      : ACES::Protocol(name, hw, pri, UpdateFreq){}

    ACES::Credentials* Protocol::parseHWInput(
                       ACES::Message* c) {}

    void Protocol::aggregateRequests(
      std::list<ACES::Credentials*> &reqs){
        while( not reqs.empty() ){
            ACES::Message *m = new ACES::Message( reqs.front() );
            reqs.pop_front();
            pending_stack->push_back(m);
        }
    }

    Parameter::Parameter(std::string n, ACES::Credentials* c,
          ACES::Dispatcher* d, int pri, int UpdateFreq)
      : ACES::Parameter(n, c, d, pri, UpdateFreq)
      {
         //RTT::Method<void(float)>* setMethod = new RTT::Method<void(float)>
         //   ("setMethod", &Parameter::set, this);
         //this->methods()->addMethod(setMethod,
         //   "Set the goal for the parameter", "value", "set point");
      }

/*
    bool Parameter::setGoal(ACES::PValue* val){
        return ACES::Parameter::setGoal(val);
    }

    bool Parameter::goalSet(){
        return ACES::Parameter::goalSet();
    }
*/
};
