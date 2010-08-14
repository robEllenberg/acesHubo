#include "hardware.hpp"

namespace ACES {

    Hardware::Hardware(std::string cfg, std::string args) :
        taskCfg(cfg),
        RTT::TaskContext(name),
        txUpStream("txUpStream"),
        dsQueue(),
        usQueue()
    {
        this->events()->addEvent(&txUpStream, "txUpStream", "word",
                                 "Recieved Data");

        this->setActivity( new RTT::Activity( priority, 1.0/freq, 0,
                                              name));
    }
/*    
    Hardware::Hardware(std::string name)
      : RTT::TaskContext(name)
    {
         this->events()->addEvent(&txUpStream, "txUpStream", "word",
                                 "Recieved Data");
    }
*/
    bool Hardware::rxDownStream(Message* m){
        RTT::OS::MutexLock lock(dsqGuard);
        dsQueue.push_back(m);
        //m->printme();
        return true;
    }

    void Hardware::updateHook(){
        //RTT::Logger::log() << "hw sent" << RTT::endlog();
        Message* m = NULL;
        while(dsQueue.size()){
            m = processDSQueue();
            //RTT::Logger::log() << "hw sent" << RTT::endlog();
            txBus(m);
        }
        rxBus();
        ProtoWord* p = NULL;
        while(usQueue.size()){
            p = processUSQueue();
            txUpStream(p);
        }
    }

    Message* Hardware::processDSQueue(){
        RTT::OS::MutexLock lock(dsqGuard);
        Message* m = dsQueue.front();
        dsQueue.pop_front();
        return m;
    }

    ProtoWord* Hardware::processUSQueue(){
        RTT::OS::MutexLock lock(usqGuard);
        ProtoWord* p = usQueue.front();
        usQueue.pop_front();
        return p;
    }

    bool Hardware::txBus(Message* m){
        Goal* g = NULL;
        if( m->goalList.size() ){
            for(std::list<Goal*>::iterator it = m->goalList.begin();
                it != m->goalList.end();
                it++)
            {
                ACES::ProtoWord* w =
                    (ACES::ProtoWord*)(new ACES::Word<Goal*>(*it));

                { RTT::OS::MutexLock lock(usqGuard);
                  usQueue.push_back(w);
                }
            }
            return true;
        }
        else{
            return false;
        }
    }
    
    bool Hardware::rxBus(){
        return true;
    }

    bool Hardware::subscribeProtocol(Protocol* p){
        this->connectPeers( (RTT::TaskContext*) p);
        RTT::Handle h = p->events()->setupConnection("txDownStream")
            .callback( this, &Hardware::rxDownStream
            //           ,p->engine()->events() ).handle();
            ).handle();
        if(! h.ready() ){
            return false;
        }
        h.connect();
        if(!h.connected() ){
            return false;
        }

        h = this->events()->setupConnection("txUpStream")
            .callback( p, &Protocol::rxUpStream
            //           ,this->engine()->events() ).handle();
            ).handle();

        if(! h.ready() ){
            return false;
        }
        h.connect();
        if(!h.connected() ){
            return false;
        }
 
        return true;
    }

/*
    charDevHardware::charDevHardware(std::string name,
                       std::ifstream *in,
                       std::ofstream *out, int priority,
                       int UpdateFreq) : 
            Hardware(name, priority, UpdateFreq),
            outBuffer("Out", 10),
            inBuffer("In")
        {
            hardpoint_in = in;
            hardpoint_out = out;
        };

    void charDevHardware::updateHook(){
        this->checkForLineData();
        this->checkForProtocolData();
    }

    void charDevHardware::checkForProtocolData(){
        unsigned char c;
        unsigned char count = 0;
        Message* cpv;
        while( this->inBuffer.size() and (count < 8) ){
            this->inBuffer.Pop(cpv);
            //RTT::Logger::log() << "Protocol Sent: "
            //<< c << std::endl;

            //this->hardpoint_out->put(
            //        ((Message*)cpv)->cval);
            count++;
        }
        this->hardpoint_out->flush();
    }

    void charDevHardware::checkForLineData(){
        unsigned char c;
        while( this->hardpoint_in->readsome((char*)&c, 1) ){
            //TODO - Reimplement, post removal of SVal's
            //charDevSValue* cpv = new charDevSValue(c);
            //outBuffer.buffer()->Push((Message*)cpv);	
	    }
    }
*/
}
