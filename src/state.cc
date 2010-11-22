namespace ACES{

    template <class T>
    History<T>::History(int s)
     :hist(s)
    {
        size = s;
        lastValid = -1;
    }

    template <class T>
    History<T>::History(int s, Sample<T> ic)
     :hist(s)
    {
        size = s;
        hist[0] = ic;
        lastValid = 0;
    }
    
    template <class T>
    void History<T>::update(T value)
    {
        Sample<T> s(value, RTT::os::TimeService::Instance()->getTicks());
        hist.push_front(s);
        hist.pop_back();
    }

    template <class T>
    Sample<T> History<T>::getSample(int sampleNum){
    //need a way to return on invalid sample
        return hist[sampleNum];
    }

    template <class T>
    Sample<T> History<T>::getSampleSec(float sec){
        //TODO
        return 0.;
    }

    template <class T>
    Sample<T> History<T>::getSampleTicks(RTT::os::TimeService::ticks t){
        //TODO
        return Sample<T>();
    }

    template <class T>
    void History<T>::printme(){
        RTT::Logger::log() << "Sample [n]: t, val" << RTT::endlog();
        for(int i=0; i < hist.size(); i++){
            Sample<T> s = hist[i];
            RTT::Logger::log() << i << ": " << s.getSec() << ", "
                               << s.getVal() << RTT::endlog();
        }
    }
    
    template <class T>
    Sample<T>::Sample(){
        valid = false;
    }

    template <class T>
    Sample<T>::Sample(T val, RTT::os::TimeService::ticks time){
        value = val;
        t = time;
        valid = true;
    }

    template <class T>
    bool Sample<T>::isValid(){
        return valid;
    }

    template <class T>
    T Sample<T>::getVal(){
        return value;
    }

    template <class T>
    RTT::os::TimeService::ticks Sample<T>::getTick(){
        return t;
    }

    template <class T>
    float Sample<T>::getSec(){
        return RTT::os::TimeService::Instance()->getSeconds(t);
    }

    template <class T>
    State<T>::State(std::string cfg, int nID) :
      ProtoState(cfg, nID),
      hist(10),
      value(0)
    {
        this->addOperation("sample", &State<T>::sample, this, RTT::OwnThread
                          ).doc("Sample the State");
        this->addOperation("go", &State<T>::go, this, RTT::OwnThread).doc(
                           "Go to a specified Setpoint").arg("SP", "Set Point");
        this->addOperation("printHist", &State<T>::printHistory, this,
                           RTT::OwnThread).doc("Print the history");
        //TODO - Function to print out the current history
        //this->addOperation("hist", &State<T>::getHist, this, RTT::OwnThread).doc(
        //                   "Get a historical sample").arg("sample#",
        //                   "Number of sample in sequence");

        this->addAttribute("value", value);
        this->addAttribute("integral", integral);
        this->addAttribute("diff", diff);

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
            }
        }
    }

    template <class T>
    float State<T>::getInt(){
        return integral;
    }

    template <class T>
    float State<T>::getDiff(){
        return diff;
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
    void State<T>::printHistory(){
        hist.printme();
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
                           << this->name << ") Value: " << w->getData();
        hist.update( w->getData() );
        value = w->getData();

        if(intEnable){
            updateInt(hist.getSample(0), hist.getSample(1));
        }
        if(diffEnable){
            updateDiff(hist.getSample(0), hist.getSample(1));
        }

    }

    template <class T>
    bool State<T>::updateInt(Sample<T> cur, Sample<T> last){
        if(cur.isValid() and last.isValid()){
            double newArea = 1./2.*(double)( cur.getVal()+last.getVal()) *
            (double)( last.getSec() - cur.getSec() );
            integral += newArea;
            return true;
        }
        return false;
    }

    template <class T>
    bool State<T>::updateDiff(Sample<T> cur, Sample<T> last){
        if(cur.isValid() and last.isValid()){
            double t = last.getSec() - cur.getSec();
            if(t < diffThreshold){
                //deal w/the possibility of floating point error in time
                //acquisition
                diff = 0;
            }
            else{
                diff = (double)(cur.getVal()-last.getVal())/t;
            }
            return true;
        }
        return false;
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
