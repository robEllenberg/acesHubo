namespace ACES{
    template <class HW, class P>    
    void Protocol<HW,P>::updateHook(){
        assert(dsQueue.size() < 100);
        while(not dsQueue.isEmpty() ){
            Message<HW> m = processDSQueue();
            if(m){
                RTT::Logger::log() << RTT::Logger::Debug
                                   << "(Protocol) got DS" << RTT::endlog();
                //m->printme();
            }
            txDownStream(m);
        }

        assert( usQueue->size() < 100);
        while(usQueue.size()){
            PDWord<P> r = processUSQueue();
            if(r){
                //r->printme();
                RTT::Logger::log() << RTT::Logger::Debug
                                   << "(Protocol) got US" << RTT::endlog();
                txUpStream(r);
            }
        }
    }

    template <class HW, class P>    
    Message<HW>* Protocol<HW, P>::processDSQueue(){
        //Goal* g = getDSQelement();
        PDWord<P>* g = NULL;
        dsQueue.dequeue(g);
        Message<HW>* m = new Message<HW>();
        m->Push(g);
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
