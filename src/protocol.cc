namespace ACES{
    template <class HW, class P>    
    void Protocol<HW,P>::updateHook(){
        Message<HW>* m = NULL;
        while( m = processDSQueue() ){
            RTT::Logger::log() << RTT::Logger::Debug
                               << "(Protocol) got DS" << RTT::endlog();
            txDownStream(m);
        }

        PDWord<P>* r = NULL;
        while(r = processUSQueue()){
            RTT::Logger::log() << RTT::Logger::Debug
                               << "(Protocol) got US" << RTT::endlog();
            txUpStream(r);
        }
    }

    template <class HW, class P>    
    Message<HW>* Protocol<HW, P>::processDSQueue(){
        //Goal* g = getDSQelement();
        PDWord<P>* w = NULL;
        dsQueue.dequeue(w);
        Message<HW>* m = new Message<HW>();
        HWord<HW> h = new HWord<HW>()
        m->Push(w);
        return m;
    }

    template <class HW, class P>    
    PDWord<P>* Protocol<HW, P>::processUSQueue(){
        HWord<HW>* p = NULL;
        usQueue.dequeue(p);
        //TODO - Specialized processing function from HWord->PDWord
        assert(false); //TODO enforcement

        //Word<Goal*>* w = (Word<Goal*>*)p;
        //Goal* g = w->data;
        //int nID = g->nodeID;
        //Result<Goal*>* r = new Result<Goal*>(g, g->cred, nID);
        //return (ProtoResult*)r;
        return NULL;
    }

}
