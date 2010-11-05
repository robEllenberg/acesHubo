namespace ACES{
    template <class HW, class PD>    
    void Protocol<HW,PD>::updateHook(){
        Message<HW>* m = NULL;
        while( not dsQueue.isEmpty()){
            if( m = processDSQueue() ){
                RTT::Logger::log() << RTT::Logger::Debug
                                   << "(Protocol) got DS" << RTT::endlog();
                txDownStream(m);
            }
        }

        Word<PD>* r = NULL;
        while( not usQueue.isEmpty() ){
            if(r = processUSQueue()){
                RTT::Logger::log() << RTT::Logger::Debug
                                   << "(Protocol) got US" << RTT::endlog();
                txUpStream(r);
            }
        }
    }

    template <class HW, class PD>    
    Message<HW>* Protocol<HW, PD>::processDSQueue(){
        //Goal* g = getDSQelement();
        Word<PD>* w = NULL;
        dsQueue.dequeue(w);
        Message<HW>* m = new Message<HW>();
        m->Push(w);
        return m;
    }

    template <class HW, class PD>    
    Word<PD>* Protocol<HW, PD>::processUSQueue(){
        Word<HW>* h = NULL;
        usQueue.dequeue(h);

        //TODO - Specialized processing function from HWord->PDWord
        //If we're going to implement some kind of state machine or whatever
        //it needs to be done here. This default is only meaningful for type-same
        //protocols & hardware (not many at all)
        Word<PD>* p = h;

        return p;
    }

}
