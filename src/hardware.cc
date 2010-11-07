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
    Word<T>* Hardware<T>::processUSQueue(){
        Word<T>* w = NULL; 
        usQueue.dequeue(w);
        return w;
    }

    template <class T>
    bool Hardware<T>::processDSQueue(Message<T>* m){
        //Message<T>* m = NULL;
        //dsQueue.dequeue(m);
        return true;
    }

    template <class T>
    void Hardware<T>::updateHook(){
        Message<T>* m = NULL;
        while( rxDownStream.read(m) == RTT::NewData ){
            processDSQueue(m){
            RTT::Logger::log() << RTT::Logger::Debug << "(HW) got DS"
                               << RTT::endlog();
            txBus(m);
        }
        rxBus();
        Word<T>* p = NULL;
        while( not usQueue.isEmpty()){
            if(p = processUSQueue()){
                RTT::Logger::log() << RTT::Logger::Debug << "(HW) got US"
                                   << RTT::endlog();
                txUpStream(p);
            }
        }
    }

    template <class T>
    bool Hardware<T>::txBus(Message<T>* m){
        while( m->size() ){
            Word<T>* w = m->Pop();
            TxUS.write(w);
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
        RTT::PortInterface* port = NULL;
        bool success;
        RTT::ConnPolicy policy = RTT::ConnPolicy::buffer(10);

        port = (RTT::PortInterface*)p->ports()->getPort("RxUS");
        success = this->txUpStream.connectTo(port, policy);
        if(not success){
            return false;
        }
        
        port = (RTT::PortInterface*)p->ports()->getPort("TxDS");
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
