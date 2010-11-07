namespace ACES{

    template <class S, class PD>
    void Device<S,PD>::updateHook(){
        Word<PD>* g = NULL;
        while( not dsQueue.isEmpty() ){
            if(g = processDSQueue()){
                RTT::Logger::log() << RTT::Logger::Debug << "(dev) got DS"
                                   << RTT::endlog();
                txDownStream(g);
            }
        }
        
        Word<S>* p;
        while( not usQueue.isEmpty() ){
            if( p = processUSQueue() ){
                RTT::Logger::log() << RTT::Logger::Debug << "(dev) got US"
                                   << RTT::endlog();
            //typename std::deque< PDWord<P>* >::iterator it;
            //for(it = p.begin();
            //    it != p.end();  it++){
                //(*it)->printme();
                txUpStream(*p);
                //RTT::Logger::log() << "(dev) nID:" << (*it)->nodeID
                //                   << RTT::endlog();
            }
        }
    }


    template <class S, class PD>
    Word<S>* Device<S,PD>::processUSQueue(){
        Word<PD>* p = NULL;
        usQueue.dequeue(p);
        //Only works on equiv types
        return p; 
    }

    template <class S, class PD>
    Word<PD>* Device<S,PD>::processDSQueue(){
        Word<S>* s = NULL;
        dsQueue.dequeue(s);
        //Only works on equiv types
        return s; 
    }

    template <class S, class P>
    bool Device<S,P>::printCred(){
        credentials->printme();
    }

}
