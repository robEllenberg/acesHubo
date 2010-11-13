namespace ACES{

    template <class T>
    State<T>::State(std::string cfg, int nID) :
      ProtoState(cfg, nID),
      //goMethod("go", &State<T>::go, this),
      value(0)
      //txDownStream("txDownStream"),
      //dsQueue(10),
      //usQueue(10),
      //sampleMethod("sample", &State<T>::sample, this)
    {
        //this->events()->addEvent(&txDownStream, "txDownStream", "credentials",
        //    "credentials associated w/the goal");
        this->addOperation("sample", &State<T>::sample, this, RTT::OwnThread
                          ).doc("Sample the State");

        this->addOperation("go", &State<T>::go, this, RTT::OwnThread).doc(
                           "Go to a specified Setpoint").arg("SP", "Set Point");
        //dsQueue = new RTT::Buffer< std::map<std::string, void*>* >(50);
        //dsQueue = new RTT::Buffer< Goal* >(50);
        //dsQueue = new RTT::Buffer< Goal* >(50);

        this->addAttribute("value", value);
        asgnfunct = assign;

        this->ports()->addEventPort("RxDS", rxDownStream).doc(
                               "DownStream (from Controller) Reception");
        this->ports()->addPort("RxUS", rxUpStream).doc(
                               "UpStream (from Device) Reception");
        this->ports()->addPort("TxDS", txDownStream).doc(
                               "DownStream (to Device) Transmission");

        this->setActivity( new RTT::Activity( priority, 1.0/freq, 0, name) );
    }

    template <class T>
    void State<T>::updateHook(){
        if( samplingAttr ){
            sample();
        }
        std::map<std::string, void*>* dsIn = NULL;
        Word<T>* dsOut = NULL;
        while ( rxDownStream.read(dsIn) == RTT::NewData ){
            if( (dsOut = processDS(dsIn)) ){
                txDownStream.write(dsOut);
            }
        }
        
        Word<T>* usIn = NULL;
        while( rxUpStream.read(usIn) == RTT::NewData ){
            RTT::Logger::log() << RTT::Logger::Debug << "(state: " 
                               << name << ") rxUS"
                               << RTT::endlog();
            //  RTT::Logger::log() <<  "r nid =" << rx->nodeID << " my nid="
            //                     << nodeIDAttr.get() << RTT::endlog();
            
            if(usIn->getNodeID() == nodeID){
                RTT::Logger::log() << RTT::Logger::Debug << "(state: "
                                   << name << ") assign"
                                   << RTT::endlog();
                assign(usIn);
                //asgnfunct(usIn, this);
            }
        }
    }

    template <class T>
    void State<T>::sample(){
        RTT::Logger::log() << RTT::Logger::Debug << "(state: "
                           << name << ") sample"
                           << RTT::endlog();
        Word<T>* s = new Word<T>(0, nodeID, 0, REFRESH);
        txDownStream.write(s);
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
        Word<T>* w = new Word<T>(sp, nodeID, 0, SET);
        txDownStream.write(w);
    }

    template <class T>
    void State<T>::assign(Word<T>* w){
        RTT::Logger::log() << RTT::Logger::Debug
                           << "(state: "
                           << th->name << ") Value: " << w->getData();
        value = w->getData();
    }

    template <class T>
    Word<T>* State<T>::processDS( std::map<std::string, void*>* p ){
       //RTT::Logger::log() << "trigger " + name << RTT::endlog();
       std::map<std::string, void* >::iterator mypair;
        mypair = p->find( name );
        if(mypair != p->end() ){
            void* pval = (*mypair).second;
            T val = *((T*)pval);
            Word<T>* w = new Word<T>(val, nodeID, 0, SET);
            return w; 
        }
        return NULL;
    }

    template <class T>
    std::string State<T>::logVal(){
        std::stringstream s;
        s << this->value;
        return s.str();
    }
}
