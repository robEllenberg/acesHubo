namespace ACES {
    template <typename T>
    Hardware<T>::Hardware(std::string cfg, std::string args) :
        ProtoHardware(cfg, args),
        //txUpStream("txUpStream"),
        //dsQueue(10),
        usQueue(10)
    {
        //this->events()->addEvent(&txUpStream, "txUpStream", "word",
        //                         "Recieved Data");

        this->ports()->addPort("TxUS", txUpStream).doc(
                               "UpStream (to Protocol) Transmission");
        this->ports()->addEventPort("RxDS", rxDownStream).doc(
                               "DownStream (from Protocol) Reception");

        this->setActivity( new RTT::Activity( priority, 1.0/freq, 0, name ) );
    }

    template <typename T>
    bool Hardware<T>::processUS(Word<T>* usIn){
        return true;
    }

    template <class T>
    bool Hardware<T>::processDS(Message<T>* dsIn){
        return true;
    }

    template <class T>
    void Hardware<T>::updateHook(){
    
        Message<T>* dsIn = NULL;
        while( rxDownStream.read(dsIn) == RTT::NewData ){
            if( processDS(dsIn) ){
                RTT::Logger::log() << RTT::Logger::Debug << "(HW: " << name 
                                   << ") got DS"
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
                RTT::Logger::log() << RTT::Logger::Debug << "(HW: " 
                                   << name << ") got US"
                                   << RTT::endlog();
                txUpStream.write(usIn);
            }
        }
        
    }

    template <class T>
    bool Hardware<T>::txBus(Message<T>* m){
        while( m->size() ){
            Word<T>* w = m->pop();
            txUpStream.write(w);
            //usQueue.enqueue(w);
            //TODO - Delete the word
        }
        return true;
    }

    template <class T>
    void Hardware<T>::rxBus(){
    }

    /*
    template <class T>
    bool Hardware<T>::rxDownStream(Message<T>* m){
        dsQueue.enqueue(m);
        return true;
    }
    */


}
