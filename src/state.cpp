#include "state.hpp"
namespace ACES {

    ProtoState::ProtoState(std::string config, std::string args) :
      taskCfg(config),
      RTT::TaskContext(name),
      announceGoal("announceGoal")
    {
        this->events()->addEvent(&announceGoal, "announceGoal", "credentials",
            "credentials associated w/the goal");

        if(args == "Joint"){
            propID = JOINT;
        }

      //set_stack = new RTT::Buffer< std::map<std::string, void*>* >(50);
      set_stack = new RTT::Buffer< Goal* >(50);

        this->setActivity(
                new RTT::Activity( priority, 1.0/freq, 0, name)
        );
        
    }
    
    bool ProtoState::configureHook(){
        return true;
    }
    
    bool ProtoState::startHook(){
        return true;
    }
    
    void ProtoState::updateHook(){
        Goal* g = new Goal(this->propID, REFRESH);
        set_stack->Push(g);
        //g->printme();
        //RTT::Logger::log() << "Update State "
        //<< this->name << RTT::Logger::endl;
        while ( not set_stack->empty() ){
            Goal* h;
            set_stack->Pop(h);
            announceGoal(h);
        }
    }
    
    void ProtoState::stopHook(){
    }
    
    void ProtoState::cleanupHook(){
    }

    bool ProtoState::subscribeController(Controller* c){
        this->connectPeers( (RTT::TaskContext*) c);
        RTT::Handle h = c->events()->setupConnection("applyStateVector")
                .callback( this, &ProtoState::setGoal,
                           c->engine()->events() ).handle();
        if( not h.ready() ){
            return false;
        }
        h.connect();
        if( not h.connected() ){
            return false;
        }
        return true;
    }

    void ProtoState::setGoal(std::map<std::string, void*>* p){
       //RTT::Logger::log() << "trigger" << RTT::endlog();
       std::map<std::string, void* >::iterator mypair;
        mypair = p->find( name );
        if(mypair != p->end() ){
            void* val = (*mypair).second;
            Goal* g = new Goal(propID, SET, val);
           
            //RTT::Logger::log() << this->name << " Announce: "
            //<< *((float*)val) << RTT::endlog();
            //announceGoal(g);
            set_stack->Push(g);
        }
    }

    void ProtoState::RxData(Goal* g){
        //if(name == "RSP"){
        //    RTT::Logger::log() << *((float*)(g->data)) << RTT::endlog();
        //}
            //if(name == "RKP"){
            //    g->printme();
            //}
        asgnfunct(g->data, this);
    }

}
