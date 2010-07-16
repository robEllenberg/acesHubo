#include "state.hpp"
namespace ACES {
    ProtoState::ProtoState(std::string n, int pid,
      int pri, int UpdateFreq)
      : RTT::TaskContext(n),
        announceGoal("announceGoal")
    {
        name = n;
        propID = pid;
        frequency = UpdateFreq;
        priority = pri;

        this->events()->addEvent(&announceGoal, "announceGoal", "credentials",
            "credentials associated w/the goal");

        this->setActivity(
            new RTT::Activity( priority, 1.0/UpdateFreq, 0, n )
        );
    }

    ProtoState::ProtoState(taskCfg cfg, int pid) :
      RTT::TaskContext(cfg.name), announceGoal("announceGoal"){

        this->events()->addEvent(&announceGoal, "announceGoal", "credentials",
            "credentials associated w/the goal");

        name = cfg.name;
        priority = cfg.priority;
        frequency = cfg.freq;
        propID = pid;

        this->setActivity(
            new RTT::Activity( priority, 1.0/frequency, 0, cfg.name )
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
        //g->printme();
        announceGoal(g);
        //RTT::Logger::log() << "Update State "
        //<< this->name << RTT::Logger::endl;

    }
    
    void ProtoState::stopHook(){
    }
    
    void ProtoState::cleanupHook(){
    }

    bool ProtoState::subscribeController(WbController* c){
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
            //T* valp = (*mypair).second;
            //T val = *valp;
            void* val = (*mypair).second;
            Goal* g = new Goal(propID, SET, val);
            //Credentials* c = credentials->copy(val);
            //Credentials* c = new Credentials(this->credentials, val);
            
            //RTT::Logger::log() << this->name << " Announce: "
            //<< *((float*)val) << RTT::endlog();
            announceGoal(g);
        }
    }

}
