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
        while( this->pending_stack->size() ){
            //pop next message off pending  
           issueMessage( this->prepareMessage() );
        }
    }

    template <class T>
    void Protocol::addRequest(Credentials<T>* c){
        ACES::Message *m = new ACES::Message( c );
        pending_stack->push_back(m);
    }

    void Protocol::stopHook(){
    }
    
    void Protocol::cleanupHook(){
    }

    Message* Protocol::prepareMessage(){
        Message* m = this->pending_stack->front();
        this->pending_stack->pop_front();
        return m;
        //m->printme();
    }
    
    template <class T>
    bool Protocol::subscribeState(State<T>* p){
        this->connectPeers( (RTT::TaskContext*) p );

        RTT::Handle h = p->events()->setupConnection("announceGoal")
            .callback( this, &Protocol::addRequest,
                       this->engine()->events() ).handle();
        assert( h.ready() );
        h.connect();
        assert( h.connected() );
        return true;
    }

}

