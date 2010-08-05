#include "state.hpp"
namespace ACES {

    ProtoState::ProtoState(std::string config, std::string args) :
      taskCfg(config),
      RTT::TaskContext(name),
      txDownStream("txDownStream")
    {
        this->events()->addEvent(&txDownStream, "txDownStream", "credentials",
            "credentials associated w/the goal");

        if(args == "Joint"){
            nodeID = JOINT;
        }

      //set_stack = new RTT::Buffer< std::map<std::string, void*>* >(50);
      set_stack = new RTT::Buffer< Goal* >(50);

        this->setActivity(
                new RTT::Activity( priority, 1.0/freq, 0, name)
        );
        
    }
   
    void ProtoState::updateHook(){
        Goal* g = new Goal(this->nodeID, REFRESH, new float(3.14));
        set_stack->Push(g);
        //g->printme();
        //RTT::Logger::log() << "Update State "
        //<< this->name << RTT::Logger::endl;
        while (!set_stack->empty() ){
            Goal* h;
            set_stack->Pop(h);
            txDownStream(h);
        }
    }

    bool ProtoState::subscribeController(Controller* c){
        this->connectPeers( (RTT::TaskContext*) c);
        RTT::Handle h = c->events()->setupConnection("applyStateVector")
                .callback( this, &ProtoState::rxDownStream,
                           c->engine()->events() ).handle();
        if(!h.ready() ){
            return false;
        }
        h.connect();
        if(!h.connected() ){
            return false;
        }
        return true;
    }

    void ProtoState::rxDownStream(std::map<std::string, void*>* p){
       //RTT::Logger::log() << "trigger" << RTT::endlog();
       std::map<std::string, void* >::iterator mypair;
        mypair = p->find( name );
        if(mypair != p->end() ){
            void* val = (*mypair).second;
            Goal* g = new Goal(nodeID, SET, val);
           
            //RTT::Logger::log() << this->name << " Announce: "
            //<< *((float*)val) << RTT::endlog();
            //txDownStream(g);
            set_stack->Push(g);
        }
    }

    void ProtoState::rxUpStream(ProtoResult* r){
        //if(name == "RSP"){
        //    RTT::Logger::log() << *((float*)(g->data)) << RTT::endlog();
        //}
            //if(name == "RKP"){
            //    g->printme();
            //}
        if(r->nodeID == nodeID){
            asgnfunct(r, this);
        }
    }

}
