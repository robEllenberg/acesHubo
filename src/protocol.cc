namespace ACES{
    template <class HW, class PD>    
    Protocol<HW,PD>::Protocol(std::string cfg, std::string args) :
      taskCfg(cfg),
      RTT::TaskContext(name)
      //txDownStream("txDownStream"),
      //txUpStream("txUpStream"),
      //dsQueue(10),
      //usQueue(10)
    {
        //requestBuf = new std::deque<Message*>;

        //TODO - Figure out why this flips out when we attempt to declare
        // it as a member instead of a pointer 
        //returnBuf = new RTT::Buffer<ProtoResult*>(250);
        //returnQueue = new std::deque<ProtoResult*>(250);
        //requestQueue = new std::deque<Goal*>(250);
        /*
        this->events()->addEvent(&txDownStream, "txDownStream", "msg",
                                 "The message to be transmitted");
        this->events()->addEvent(&txUpStream, "txUpStream", "result",
                                 "Data struct containing processed result");
        */
        this->ports()->addPort("RxDS", rxDownStream).doc(
                               "DownStream (from Device) Reception");
        this->ports()->addPort("RxUS", rxUpStream).doc(
                               "UpStream (from Hardware) Reception");
        this->ports()->addPort("TxDS", txDownStream).doc(
                               "DownStream (to Hardware) Transmission");
        this->ports()->addPort("TxUS", txUpStream).doc(
                               "UpStream (to Device) Transmission");
        this->setActivity(
            new RTT::Activity( priority, 1.0/freq, 0, name )
        );
    }

    template <class HW, class PD>    
    void Protocol<HW,PD>::updateHook(){
        Word<PD>* p = NULL;
        Message<HW>* h = NULL;
        while( rxDownStream.read(p) == RTT::NewData ){
            if( h = processDS(p) ){
                RTT::Logger::log() << RTT::Logger::Debug
                                   << "(Protocol) got DS" << RTT::endlog();
                txDownStream.write(m);
            }
        }

        Word<HW>* h2 = NULL;
        Word<PD>* p2 = NULL;
        while( rxUpStream.read(h2) == RTT::NewData ){
            if( p2 = processUS(h2) ){
                RTT::Logger::log() << RTT::Logger::Debug
                                   << "(Protocol) got US" << RTT::endlog();
                txUpStream.write(p2);
            }
        }
    }

    template <class HW, class PD>    
    Message<HW>* Protocol<HW, PD>::processDS(Word<PD>* p){
        Message<HW>* m = NULL;
        m = new Message<HW>();
        m->Push(p);
        return m;
    }

    template <class HW, class PD>    
    Word<PD>* Protocol<HW, PD>::processUS(Word<HW>* h){
        Word<PD>* p = NULL;

        //TODO - Specialized processing function from HWord->PDWord
        //If we're going to implement some kind of state machine or whatever
        //it needs to be done here. This default is only meaningful for 
        //type-same protocols & hardware (not many at all)
        p = h;

        return p;
    }

    template <class HW, class PD>    
        template<class S>
    bool Protocol<HW,PD>::subscribeDevice(RTT::TaskContext* d){
        this->connectPeers( (RTT::TaskContext*) d );
        RTT::Handle h = d->events()->setupConnection("txDownStream")
            .callback( this, &Protocol<HW,PD>::rxDownStream,
                       d->engine()->events()
                     ).handle();
        if(!h.ready()){
            return false;
        }
        h.connect();
        if(!h.connected() ){
            return false;
        }

        h = this->events()->setupConnection("txUpStream")
            .callback( d, &Device<S, PD>::rxUpStream
        //               ,this->engine()->events()
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

    template <class HW, class PD>
    bool Protocol<HW,PD>::connectHardware(RTT::TaskContext* h){
        this->connectPeers(h);
       /* RTT::Handle hand = this->events()->setupConnection("txDownStream")
            .callback( h, &Hardware<HW>::rxDownStream
            //           ,p->engine()->events() ).handle();
            ).handle(); */
        //RTT::base
        RTT::PortInterface* p = NULL;
        bool success;
        RTT::ConnPolicy policy = RTT::ConnPolicy::buffer(10);

        p = (RTT::PortInterface*)h->ports()->getPort("RxDS");
        success = this->txDownStream.connectTo(p, policy);
        if(not success){
            return false;
        }
        
        p = (RTT::PortInterface*)h->ports()->getPort("TxUS");
        success = p->connectTo(this->rxUpStream, policy);
        if(not success){
            return false;
        }
                                  
        /*hand = h->events()->setupConnection("txUpStream")
            .callback( this, &Protocol<HW,PD>::rxUpStream
            //           ,this->engine()->events() ).handle();
            ).handle();*/
        return true;
    }

}
