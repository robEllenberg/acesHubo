namespace ACES{
    template <class HW, class PD>    
    Protocol<HW,PD>::Protocol(std::string cfg, std::string args) :
      ProtoProtocol(cfg,args) 
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
        this->ports()->addEventPort("RxDS", rxDownStream).doc(
                               "DownStream (from Device) Reception");
        this->ports()->addEventPort("RxUS", rxUpStream).doc(
                               "UpStream (from Hardware) Reception");
        this->ports()->addPort("TxDS", txDownStream).doc(
                               "DownStream (to Hardware) Transmission");
        this->ports()->addPort("TxUS", txUpStream).doc(
                               "UpStream (to Device) Transmission");
        this->setActivity(
            new RTT::Activity(this->priority, 1.0/(this->freq), 0,
                              this->name )
        );
    }

    template <class HW, class PD>    
    void Protocol<HW,PD>::updateHook(){
        Word<PD>* dsIn = NULL;
        Message<HW>* dsOut = NULL;
        while( rxDownStream.read(dsIn) == RTT::NewData ){
            if( dsOut = processDS(dsIn) ){
                RTT::Logger::log() << RTT::Logger::Debug
                                   << "(Protocol: " << name << ") got DS"
                                   << RTT::endlog();
                txDownStream.write(dsOut);
            }
        }

        Word<HW>* usIn = NULL;
        Word<PD>* usOut = NULL;
        while( rxUpStream.read(usIn) == RTT::NewData ){
            if( usOut = processUS(usIn) ){
                RTT::Logger::log() << RTT::Logger::Debug
                                   << "(Protocol: "
                                   << name << ") got US" << RTT::endlog();
                txUpStream.write(usOut);
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
}
