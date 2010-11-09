namespace ACES{
    template <class S, class PD>
    Device<S,PD>::Device(std::string config) :
      ProtoDevice(config)
      //txDownStream("txDownStream"),
      //txUpStream("txUpStream"),
      //dsQueue(10),
      //usQueue(10),
    {
        /*this->events()->addEvent(&txDownStream, "txDownStream", "goal",
                                 "The Goal/SP Data");
        this->events()->addEvent(&txUpStream, "txUpStream", "data",
                                 "Interperted data going to states");*/
        this->addOperation("credentials", &Device<S,PD>::printCred, this,
                           RTT::OwnThread).doc("Print the Credentials");
        this->ports()->addEventPort("RxDS", rxDownStream).doc(
                               "DownStream (from State) Reception");
        this->ports()->addEventPort("RxUS", rxUpStream).doc(
                               "UpStream (from Protocol) Reception");
        this->ports()->addPort("TxDS", txDownStream).doc(
                               "DownStream (to Protocol) Transmission");
        this->ports()->addPort("TxUS", txUpStream).doc(
                               "UpStream (to State) Transmission");
        this->setActivity(
            new RTT::Activity( priority, 1.0/freq, 0, name )
        );
    }

    template <class S, class PD>
    void Device<S,PD>::updateHook(){
        Word<S>* dsIn = NULL;
        Word<PD>* dsOut = NULL;
        while( rxDownStream.read(dsIn) == RTT::NewData ){
            if( dsOut = processDS(dsIn) ){
                RTT::Logger::log() << RTT::Logger::Debug << "(dev: " 
                                   << name << ") got DS"
                                   << RTT::endlog();
                txDownStream.write(dsOut);
            }
        }
        
        Word<PD>* usIn = NULL;
        Word<S>* usOut = NULL;
        while( rxUpStream.read(usIn) == RTT::NewData ){
            if( usOut = processUS(usIn) ){
                RTT::Logger::log() << RTT::Logger::Debug << "(dev: "
                                   << name << ") got US"
                                   << RTT::endlog();
            //typename std::deque< PDWord<P>* >::iterator it;
            //for(it = p.begin();
            //    it != p.end();  it++){
                //(*it)->printme();
                txUpStream.write(usOut);
                //RTT::Logger::log() << "(dev) nID:" << (*it)->nodeID
                //                   << RTT::endlog();
            }
        }
    }

    template <class S, class PD>
    Word<S>* Device<S,PD>::processUS(Word<PD>* usIn){
        Word<S>* usOut = NULL;
        if(*(usIn->getCred()) == *credentials){
            //Only works on equiv types
            return usIn; 
        }
        return NULL;
    }

    template <class S, class PD>
    Word<PD>* Device<S,PD>::processDS(Word<S>* dsIn){
        Word<PD>* dsOut = NULL;
        dsIn->setCred(credentials);
        //Only works on equiv types
        return dsIn; 
    }

    template <class S, class P>
    void Device<S,P>::printCred(){
        credentials->printme();
    }

}
