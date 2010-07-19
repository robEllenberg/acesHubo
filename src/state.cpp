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
        announceGoal(g);
        //RTT::Logger::log() << "Update State "
        //<< this->name << RTT::Logger::endl;

    }
    
    void ProtoState::stopHook(){
    }
    
    void ProtoState::cleanupHook(){
    }

    bool ProtoState::subscribeController(Controller* c){
        this->connectPeers( (RTT::TaskContext*) c);
        RTT::Handle h = c->events()->setupConnection("applyStateVector")
                .callback( this, &ProtoState::setGoal,
                           this->engine()->events() ).handle();
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
        std::map<std::string, void* >::iterator mypair;
        mypair = p->find( name );
        if(mypair != p->end() ){
            void* val = (*mypair).second;
            Goal* g = new Goal(propID, SET, val);
           
            //RTT::Logger::log() << this->name << " Announce: "
            //<< *((float*)val) << RTT::endlog();
            announceGoal(g);
        }
    }

    void ProtoState::RxData(Goal* g){
        asgnfunct(g->data);
    }

}
