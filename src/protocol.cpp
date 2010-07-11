#include "protocol.hpp"

namespace ACES {
    Protocol::Protocol(std::string n, Hardware* hw,
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
            new RTT::Activity( priority, 1.0/frequency, 0, n )
        );
    }

    bool Protocol::addHW(Hardware* hw){
        RTT::Handle h = this->events()->setupConnection("issueMessage")
            .callback( hw, &Hardware::transmit
           //            hw->engine()->events() ).handle();
            ).handle();
        if(not h.ready() ){
            return false;
        }
        h.connect();
        if( not h.connected() ){
            return false;
        }
        return true;
    }

    
    bool Protocol::configureHook(){
        return true;
    }
    
    bool Protocol::startHook(){
        //RTT::Logger::log() << "Protocol Startup"
        //                   << std::endl;
        /*
        for(std::list<void*>::iterator it = pramlist.begin();
            it !=pramlist.end(); it++){
                //TODO - Have no idea if this will hold
                // in all cases, better check that out
                ((State<int>*)(*it))->start();
        }
        */
        return true;
    }

    bool Protocol::theresStillTime(){
        return true;
    }

    //void Protocol::updateHook(const std::vector<RTT::PortInterface*>& updatedPorts)
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

/*
    std::list<Credentials*>* Protocol::getNewRequests(){
        std::list<Credentials*>* cred = new std::list<Credentials*>();
        while( this->request_stack->size() ){
            Credentials* c; 
            this->request_stack->Pop(c);
            cred->push_back(c);
        }
        return cred;
    }
*/
    template <class T>
    void Protocol::addRequest(Credentials<T>* c){
        ACES::Message *m = new ACES::Message( c );
        pending_stack->push_back(m);
    }

    void Protocol::stopHook(){
        for(std::list<void*>::iterator it = pramlist.begin();
            it !=pramlist.end(); it++){
                //TODO - Have no idea if this will hold
                // in all cases, better check that out
                ((State<int>*)(*it))->stop();
        }
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
    bool Protocol::registerState(State<T>* p){
        this->connectPeers(p);
        pramlist.push_back(p);

        RTT::Handle h = p->events()->setupConnection("sendGoal")
            .callback( this, &Protocol::addRequest,
                       this->engine()->events() ).handle();
        assert( h.ready() );
        h.connect();
        assert( h.connected() );
        return true;
    }

}

