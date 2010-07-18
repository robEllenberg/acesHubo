#include "protocol.hpp"

namespace ACES {
    Protocol::Protocol(std::string cfg, std::string args):
      taskCfg(cfg),
      RTT::TaskContext(name),
      issueMessage("issueMessage")
    {
//        pending_stack = new std::deque<Message*>;

        //TODO - Figure out why this flips out when we attempt to declare
        // it as a member instead of a pointer 
        pending_stack = new RTT::Buffer<Message*>(1000);
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
        //while( pending_stack.size() > 0){
        while( not pending_stack->empty() ){
            //pop next message off pending  

            Message* m = prepareMessage();
            issueMessage( m );
        }
    }

    void Protocol::addRequest(Goal* g){
        ACES::Message *m = new ACES::Message( g );
//        pending_stack->push_back(m);
        pending_stack->Push(m);
    }

    void Protocol::stopHook(){
    }
    
    void Protocol::cleanupHook(){
    }

    Message* Protocol::prepareMessage(){
        //Message* m = pending_stack->front();
        //pending_stack->pop_front();
        Message* m;
        pending_stack->Pop(m);
        return m;
    }
    
    bool Protocol::subscribeDevice(Device* d){
        this->connectPeers( (RTT::TaskContext*) d );
        RTT::Handle h = d->events()->setupConnection("TxRequest")
            .callback( this, &Protocol::addRequest,
                       d->engine()->events()
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

