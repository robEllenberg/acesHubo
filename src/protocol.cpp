#include "protocol.hpp"

namespace ACES {
    Protocol::Protocol(std::string cfg, std::string args):
      taskCfg(cfg),
      RTT::TaskContext(name),
      issueMessage("issueMessage"),
      announceResult("announceResult")
    {
//        requestBuf = new std::deque<Message*>;

        //TODO - Figure out why this flips out when we attempt to declare
        // it as a member instead of a pointer 
        requestBuf = new RTT::Buffer<Message*>(250);
        returnBuf = new RTT::Buffer<ProtoResult*>(250);
        
        this->events()->addEvent(&issueMessage, "issueMessage", "msg",
                                 "The message to be transmitted");
        this->events()->addEvent(&announceResult, "announceResult", "result",
                                 "Data struct containing processed result");
        this->setActivity(
            new RTT::Activity( priority, 1.0/freq, 0, name )
        );
    }

    Protocol::Protocol(std::string name)
      : RTT::TaskContext(name),
      issueMessage("issueMessage"),
      announceResult("announceResult")
    {
//        requestBuf = new std::deque<Message*>;

        //TODO - Figure out why this flips out when we attempt to declare
        // it as a member instead of a pointer 
        requestBuf = new RTT::Buffer<Message*>(250);
        returnBuf = new RTT::Buffer<ProtoResult*>(250);
        
        this->events()->addEvent(&issueMessage, "issueMessage", "msg",
                                 "The message to be transmitted");
        this->events()->addEvent(&announceResult, "announceResult", "result",
                                 "Data struct containing processed result");
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
        //      && this->requestBuf->size()
        //      && this->theresStillTime() ){
        //while( requestBuf.size() > 0){
        if( requestBuf->size() > 200){
            RTT::Logger::log() << "PCol, RequestBuff size "
            << requestBuf->size() << RTT::endlog();
        }
        if(!requestBuf->empty() ){
            //pop next message off pending  
            //m->printme();
            Message* m = prepareMessage();
            issueMessage( m );
        }
        if (returnBuf->size() > 200){
            RTT::Logger::log() << "PCol, ReturnBuff size "
            << returnBuf->size() << RTT::endlog();
        }
        if(! returnBuf->empty() ){
            
            ProtoResult* r;
            returnBuf->Pop(r);
            announceResult(r);
        }
    }

    void Protocol::addRequest(Goal* g){
        ACES::Message *m = new ACES::Message( g );
        //g->printme();
//        requestBuf->push_back(m);
        requestBuf->Push(m);
    }

    void Protocol::stopHook(){
    }
    
    void Protocol::cleanupHook(){
    }

    Message* Protocol::prepareMessage(){
        //Message* m = requestBuf->front();
        //requestBuf->pop_front();
        Message* m;
        requestBuf->Pop(m);
        return m;
    }
    
    bool Protocol::subscribeDevice(Device* d){
        this->connectPeers( (RTT::TaskContext*) d );
        RTT::Handle h = d->events()->setupConnection("TxRequest")
            .callback( this, &Protocol::addRequest,
                       d->engine()->events()
                     ).handle();
        if(!h.ready()){
            return false;
        }
        h.connect();
        if(!h.connected() ){
            return false;
        }

        h = this->events()->setupConnection("announceResult")
            .callback( d, &Device::interpretResult,
                        this->engine()->events()
                     ).handle();
        if(!h.ready()){
            return false;
        }
        h.connect();
        if(!h.connected() ){
            return false;
        }
        return true;
    }
}

