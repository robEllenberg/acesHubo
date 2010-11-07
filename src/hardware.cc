namespace ACES {
    template <typename T>
    Hardware<T>::Hardware(std::string cfg, std::string args) :
        ProtoHardware(),
        taskCfg(cfg),
        RTT::TaskContext(name),
        //txUpStream("txUpStream"),
        //dsQueue(10),
        usQueue(10)
    {
        //this->events()->addEvent(&txUpStream, "txUpStream", "word",
        //                         "Recieved Data");
        this->ports()->addPort("TxUS", txUpStream).doc(
                               "UpStream (to Protocol) Transmission");
        this->ports()->addPort("RxDS", rxDownStream).doc(
                               "DownStream (from Protocol) Reception");

        this->setActivity( new RTT::Activity( priority, 1.0/freq, 0,
                                              name));
    }

    template <typename T>
    bool Hardware<T>::processUS(Word<T>* usIn){
        return true;
    }

    template <class T>
    bool Hardware<T>::processDS(Message<T>* m){
        return true;
    }

    template <class T>
    void Hardware<T>::updateHook(){
        Message<T>* dsIn = NULL;
        while( rxDownStream.read(dsIn) == RTT::NewData ){
            if( processDS(dsIn) ){
                RTT::Logger::log() << RTT::Logger::Debug << "(HW) got DS"
                                   << RTT::endlog();
                txBus(dsIn);
            }
        }
        rxBus();
        Word<T>* usIn = NULL;
        //while( rxUpStream.read(usIn) == RTT::NewData ){
        while(not usQueue.isEmpty() ){
            usQueue.dequeue(usIn);
            if( processUS(usIn) ){
                RTT::Logger::log() << RTT::Logger::Debug << "(HW) got US"
                                   << RTT::endlog();
                txUpStream.write(usIn);
            }
        }
    }

    template <class T>
    bool Hardware<T>::txBus(Message<T>* m){
        while( m->size() ){
            Word<T>* w = m->Pop();
            txUpStream.write(w);
            //usQueue.enqueue(w);
            //TODO - Delete the word
        }
        return true;
    }

    template <class T>
    void Hardware<T>::rxBus(){
        return 0;
    }

    /*
    template <class T>
    bool Hardware<T>::rxDownStream(Message<T>* m){
        dsQueue.enqueue(m);
        return true;
    }
    */
    template <class T>
    bool Hardware<T>::subscribeProtocol(RTT::TaskContext* p){
        this->connectPeers(p);
       /* RTT::Handle hand = this->events()->setupConnection("txDownStream")
            .callback( h, &Hardware<HW>::rxDownStream
            //           ,p->engine()->events() ).handle();
            ).handle(); */
        //RTT::base
        RTT::base::PortInterface* port = NULL;
        bool success;
        RTT::ConnPolicy policy = RTT::ConnPolicy::buffer(10);

        port = (RTT::base::PortInterface*)p->ports()->getPort("RxUS");
        success = this->txUpStream.connectTo(port, policy);
        if(not success){
            return false;
        }
        
        port = (RTT::base::PortInterface*)p->ports()->getPort("TxDS");
        success = port->connectTo(this->rxDownStream, policy);
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
