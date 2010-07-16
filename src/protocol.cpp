#include "protocol.hpp"

namespace ACES {
    Protocol::Protocol(std::string n, 
                       int pri, int UpdateFreq)
      : RTT::TaskContext(n), issueMessage("issueMessage")
    {
        priority = pri;
        frequency = UpdateFreq;

        this->pending_stack = new std::deque<Message*>;
        
        this->events()->addEvent(&issueMessage, "issueMessage", "msg",
                                 "The message to be transmitted");
        this->setActivity(
            new RTT::Activity( priority, 1.0/frequency, 0, n )
        );
    }

    Protocol::Protocol(taskCfg cfg, std::string args):
      RTT::TaskContext(cfg.name),
      issueMessage("issueMessage"){

        priority = cfg.priority;
        frequency = cfg.freq;
 
        this->pending_stack = new std::deque<Message*>;
        
        this->events()->addEvent(&issueMessage, "issueMessage", "msg",
                                 "The message to be transmitted");
        this->setActivity(
            new RTT::Activity( priority, 1.0/frequency, 0, cfg.name )
        );
    }

    
    bool Protocol::configureHook(){
        return true;
    }
    
    bool Protocol::startHook(){
        //RTT::Logger::log() << "Protocol Startup"
        //                   << std::endl;
       return true;
    }

    void Protocol::updateHook(){
        //std::list<ACES::Credentials*>* c = getNewRequests();
        //aggregateRequests(*c);
        //delete c;

        //while(   (not this->hardware->isBusyMethod())
        //      && this->pending_stack->size()
        //      && this->theresStillTime() ){
        while( this->pending_stack->size() > 0){
            //pop next message off pending  
            Message* m = this->prepareMessage();
            //RTT::Logger::log() << m->goalList.size() << RTT::endlog();
            //m->goalList.front()->printme();
           issueMessage( m );
        }
    }

    void Protocol::addRequest(Goal* g){
        ACES::Message *m = new ACES::Message( g );
        pending_stack->push_back(m);
        //g->printme();       //segfault, takes longer
        //m->goalList.front()->printme();     //Core dump
        //pending_stack->back()->goalList.front()->printme();   //segfault
    }

    void Protocol::stopHook(){
    }
    
    void Protocol::cleanupHook(){
    }

    Message* Protocol::prepareMessage(){
        Message* m = this->pending_stack->front();
        this->pending_stack->pop_front();
        //m->goalList.front()->printme();
        return m;
    }
    
    bool Protocol::subscribeDevice(Device* d){
        this->connectPeers( (RTT::TaskContext*) d );
        RTT::Handle h = d->events()->setupConnection("TxRequest")
            .callback( this, &Protocol::addRequest,
                       this->engine()->events()
                     ).handle();
        if( not h.ready()){
            return false;
        }
        h.connect();
        if( not h.connected() ){
            return false;
        }
    }

}

