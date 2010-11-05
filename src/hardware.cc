namespace ACES {
    template <class T>
    Hardware<T>::Hardware(std::string cfg, std::string args) :
        ProtoHardware(cfg, args){}

    template <class T>
    bool Hardware<T>::rxBus(){
        return true;
    }

    template <typename T>
    Word<T>* Hardware<T>::processUSQueue(){
        Word<T>* w = NULL; 
        usQueue.dequeue(w);
        return w;
    }

    template <class T>
    Message<T>* Hardware<T>::processDSQueue(){
        Message<T>* m = NULL;
        dsQueue.dequeue(m);
        return m;
    }

    template <class T>
    void Hardware<T>::updateHook(){
        Message<T>* m = NULL;
        while( not dsQueue.isEmpty()){
            if(m = processDSQueue()){
                RTT::Logger::log() << RTT::Logger::Debug << "(HW) got DS"
                                   << RTT::endlog();
                txBus(m);
            }
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
            usQueue.enqueue(w);
            //TODO - Delete the word
        }
        return true;
    }

    template <class T>
    bool Hardware<T>::rxDownStream(Message<T>* m){
        dsQueue.enqueue(m);
        return true;
    }

    template <class T>
    bool Hardware<T>::subscribeController(Controller* c){
        return true;
    }

}
