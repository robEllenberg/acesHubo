namespace ACES{

    template <class T>
    State<T>::State(std::string cfg, int nID) :
      ProtoState(cfg, nID),
      goMethod("go", &State<T>::go, this),
      value("value"),
      txDownStream("txDownStream"),
      dsQueue(10),
      usQueue(10),
      sampleMethod("sample", &State<T>::sample, this)
    {
        this->events()->addEvent(&txDownStream, "txDownStream", "credentials",
            "credentials associated w/the goal");
        this->methods()->addMethod(sampleMethod, "sample");

        //dsQueue = new RTT::Buffer< std::map<std::string, void*>* >(50);
        //dsQueue = new RTT::Buffer< Goal* >(50);
        //dsQueue = new RTT::Buffer< Goal* >(50);

        this->setActivity(
                new RTT::Activity( priority, 1.0/freq, 0, name)
        );
        bool one = this->methods()->addMethod(goMethod, "go",
                                   "SP",
                                   "Set Point");
        this->attributes()->addAttribute(&value);
        asgnfunct = assign;
    }

    template <class T>
    void State<T>::updateHook(){
        if( samplingAttr.get() ){
            sample();
        }

        while (not dsQueue.isEmpty() ){
            Word<T>* h = NULL;
            dsQueue.dequeue(h);
            txDownStream(h);
        }
        
        while(not usQueue.isEmpty() ){
            Word<T>* rx;
            usQueue.dequeue(rx);

            RTT::Logger::log() << RTT::Logger::Debug << "(state) rxUS"
                               << RTT::endlog();
            //  RTT::Logger::log() <<  "r nid =" << rx->nodeID << " my nid="
            //                     << nodeIDAttr.get() << RTT::endlog();
            
            if(rx->getNodeID() == nodeID.get()){
                RTT::Logger::log() << RTT::Logger::Debug << "(state) assign"
                                   << RTT::endlog();
                asgnfunct(rx, this);
            }
        }
    }

    template <class T>
    bool State<T>::subscribeController(Controller* c){
        this->connectPeers( (RTT::TaskContext*) c);
        RTT::Handle h = c->events()->setupConnection("applyStateVector")
                .callback( this, &State<T>::rxDownStream
                          , c->engine()->events()
                        ).handle();
        if(!h.ready() ){
            assert(false);
            return false;
        }
        h.connect();
        if(!h.connected() ){
            assert(false);
            return false;
        }
        return true;
    }

    template <class T>
    void State<T>::printme(){
        RTT::Logger::log() << "State: " << name
        << ", (Freq:" << freq << ", Pri:"
        << priority << "),"
        << "(Val = " << value.get() << ")" << std::endl;
    }

    template <class T>
    void State<T>::go(T sp){
        Word<T>* w = new Word<T>(sp, nodeID.get(), 0, SET);
        dsQueue.enqueue(w);
    }

    template <class T>
    void State<T>::assign(Word<T>* w, void* me){
        //TODO - Find a better way to do this 'virtual function'
        State<T>* th = (State<T>*)me;
        RTT::Logger::log() << RTT::Logger::Debug
                           << "(state) Value: " << w->getData();
        th->value.set( w->getData() );
    }

    template <class T>
    void State<T>::rxDownStream(std::map<std::string, void*>* p){
       //RTT::Logger::log() << "trigger " + name << RTT::endlog();
       std::map<std::string, void* >::iterator mypair;
        mypair = p->find( name );
        if(mypair != p->end() ){
            void* pval = (*mypair).second;
            T val = *((T*)pval);
            Word<T>* w = new Word<T>(val, nodeID.get(), 0, SET);
            dsQueue.enqueue(w);
        }
    }

    template <class T>
    std::string State<T>::logVal(){
        std::stringstream s;
        s << this->value.get();
        return s.str();
    }
}
