namespace ACES{
    template <class S, class PD>
    void Device<S,PD>::updateHook(){
        Word<S>* dsIn = NULL;
        Word<PD>* dsOut = NULL;
        while( rxDownStream.read(dsIn) == RTT::NewData ){
            if( dsOut = processDS(dsIn) ){
                RTT::Logger::log() << RTT::Logger::Debug << "(dev) got DS"
                                   << RTT::endlog();
                txDownStream(dsOut);
            }
        }
        
        Word<PD>* usIn = NULL;
        Word<S>* usOut = NULL;
        while( rxUpStream.read(usIn) == RTT::NewData ){
            if( usOut = processUS(usIn) ){
                RTT::Logger::log() << RTT::Logger::Debug << "(dev) got US"
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
        //Only works on equiv types
        return usIn; 
    }

    template <class S, class PD>
    Word<PD>* Device<S,PD>::processDS(Word<S>* dsIn){
        Word<PD>* dsOut = NULL;
        //Only works on equiv types
        return dsIn; 
    }

    template <class S, class P>
    void Device<S,P>::printCred(){
        credentials->printme();
    }

}
