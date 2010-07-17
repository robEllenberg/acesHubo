#include "protocol.hpp"

namespace ACES {
    Protocol::Protocol(std::string cfg, std::string args):
      taskCfg(cfg),
      RTT::TaskContext(name),
      issueMessage("issueMessage")
    {
        this->pending_stack = new std::deque<Message*>;
        
        this->events()->addEvent(&issueMessage, "issueMessage", "msg",
                                 "The message to be transmitted");
        this->setActivity(
            new RTT::Activity( priority, 1.0/freq, 0, name )
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
            issueMessage( m );
        }
    }

    void Protocol::addRequest(Goal* g){
        ACES::Message *m = new ACES::Message( g );
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

