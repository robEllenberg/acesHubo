#include "protocol.hpp"

namespace ACES {
    Protocol::Protocol(std::string n, Hardware* hw,
                       int pri, int UpdateFreq)
      :RTT::TaskContext(n){

        priority = pri;
        frequency = UpdateFreq;
        name = n;
        hardware = hw;

        this->pending_stack = new std::deque<Message*>;
        request_stack = new
            RTT::ReadBufferPort<Credentials*>("Request Stack");
        
        this->ports()->addPort(request_stack,
                               (std::string) "Request Stack");

        hwInBuffer = new
           RTT::ReadBufferPort<Message*>("FromLine");
        hwOutBuffer = new
           RTT::WriteBufferPort<Message*>("ToLine",500);
        this->ports()->addPort(hwInBuffer,
                               (std::string)"FromLine");
        this->ports()->addPort(hwOutBuffer,
                               (std::string)"ToLine");
        (*hw).outBuffer.connectTo(this->hwInBuffer);
        this->hwOutBuffer->connectTo( &(hw->inBuffer) );
        

        this->setActivity(
            new RTT::Activity( priority, 1.0/frequency )
        );
    }
    
    bool Protocol::configureHook(){
        return true;
    }
    
    bool Protocol::startHook(){
        RTT::Logger::log() << "Protocol Startup"
                           << std::endl;
        this->hardware->start();
        return true;
    }

    bool Protocol::theresStillTime(){
        return true;
    }

    //void Protocol::updateHook(const std::vector<RTT::PortInterface*>& updatedPorts)
    void Protocol::updateHook(){
        std::list<ACES::Credentials*>* c = getNewRequests();
        aggregateRequests(*c);
        delete c;
        //while(   (not this->hardware->isBusyMethod())
        //      && this->pending_stack->size()
        //      && this->theresStillTime() ){
        while( this->pending_stack->size() ){
            //pop next message off pending  
            this->issueMessage();
        }
    }

    std::list<Credentials*>* Protocol::getNewRequests(){
        std::list<Credentials*>* cred = new std::list<Credentials*>();
        while( this->request_stack->size() ){
            Credentials* c; 
            this->request_stack->Pop(c);
            cred->push_back(c);
        }
        return cred;
    }

    void Protocol::stopHook(){
        this->hardware->stop();
    }
    
    void Protocol::cleanupHook(){
    }

    void Protocol::issueMessage(){
        Message* m = this->pending_stack->front();
        this->pending_stack->pop_front();
        hwOutBuffer->Push(m);	
        //m->printme();
    }
}

