//#include "hardware.hpp"
namespace ACES {

    template <class T>
    bool Hardware<T>::rxBus(){
        return true;
    }

    template <class T>
    bool Hardware<T>::subscribeProtocol(Protocol<T>* p){
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
            .callback( p, &Protocol<T>::rxUpStream
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

    template <class T>
    Hardware<T>::Hardware(std::string cfg, std::string args) :
        taskCfg(cfg),
        RTT::TaskContext(name),
        txUpStream("txUpStream"),
        dsQueue(10),
        usQueue(10)
    {
        this->events()->addEvent(&txUpStream, "txUpStream", "word",
                                 "Recieved Data");

        this->setActivity( new RTT::Activity( priority, 1.0/freq, 0,
                                              name));
    }

/*
    template <typename T>
    Word<T> Hardware<T>::getUSQelement(){
        RTT::OS::MutexLock lock(usqGuard);
        Word<T> w = usQueue.front();
        usQueue.pop_front();
        return w;
    }

    template <class T>
    Message<T>* Hardware<T>::getDSQelement(){
        RTT::OS::MutexLock lock(dsqGuard);
        Message<T>* m = dsQueue.front();
        dsQueue.pop_front();
        return m; 
    }
*/
    template <typename T>
    Word<T> Hardware<T>::processUSQueue(){
        Word<T> w; 
        usQueue.dequeue(w);
        return w;
    }

    template <class T>
    Message<T>* Hardware<T>::processDSQueue(){
        Message<T>* m = NULL;
        dsQueue.dequeue(m);
        //getDSQelement();
        return m;
    }

    template <class T>
    void Hardware<T>::updateHook(){
        //RTT::Logger::log() << "hw sent" << RTT::endlog();
        Message<T>* m = NULL;
        while(not dsQueue.isEmpty() ){
            m = processDSQueue();
            if(m){
                RTT::Logger::log() << RTT::Logger::Debug << "(HW) got DS"
                                   << RTT::endlog();
            txBus(m);
            }
        }
        rxBus();
        Word<T> p;
        while(usQueue.size()){
            p = processUSQueue();
            if(p){
                RTT::Logger::log() << RTT::Logger::Debug << "(HW) got US"
                                   << RTT::endlog();
            txUpStream(p);
            }
        }
    }

    template <class T>
    bool Hardware<T>::txBus(Message<T>* m){
        //Goal* g = NULL;
        while( m->size() ){
            //for(std::list<Goal*>::iterator it = m->goalList.begin();
            //    it != m->goalList.end();
            //    it++)
            //{
                ACES::Word<T> w = m->Pop();
                //ACES::ProtoWord* w =
                //    (ACES::ProtoWord*)(new ACES::Word<Goal*>(*it));
                usQueue.enqueue(w);
                //{ RTT::OS::MutexLock lock(usqGuard);
                //  usQueue.push_back(w);
                //}
        }
        return true;
    }

    template <class T>
    bool Hardware<T>::rxDownStream(Message<T>* m){
        dsQueue.enqueue(m);
        //RTT::OS::MutexLock lock(dsqGuard);
        //dsQueue.push_back(m);
        //m->printme();
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
