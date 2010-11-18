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
    History<T>::update(T value)
    {
        Sample s(value, RTT::os::TimeService::Instance()->getTicks());
        hist.push_back(s);
        hist.pop_front();
    }

    Sample<T> History<T>::getSample(int sampleNum){
    //need a way to return on invalid sample
        return hist[sampleNum];
    }

    Sample<T> History<T>::getSampleSec(float sec){
        TODO
        return 0.;
    }

    Sample<T> History<T>::getSampleTicks(RTT::os::TimeSerivce::ticks t){
        TODO
        return Sample<T>();
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
        return getSeconds(t);
    }

    template <class T>
    State<T>::State(std::string cfg, int nID) :
      ProtoState(cfg, nID),
      value(0)
    {
        this->addOperation("sample", &State<T>::sample, this, RTT::OwnThread
                          ).doc("Sample the State");
        this->addOperation("go", &State<T>::go, this, RTT::OwnThread).doc(
                           "Go to a specified Setpoint").arg("SP", "Set Point");
        this->addOperation("hist", &State<T>::getHist, this, RTT::OwnThread).doc(
                           "Get a historical sample").arg("sample#",
                           "Number of sample in sequence");

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
            }
        }
        if(intEnable){
            Sample<T> cur = hist.getSample(0);
            Sample<T> last = hist.getSample(1);
            newArea = 1./2.*(double)( cur.getValue()+last.getValue()
            )*(double)(last.getSampleSec() - cur.getSampleSec())
            integral += newArea;
        }
        if(diffEnable){
            Sample<T> cur = hist.getSample(0).getVal();
            Sample<T> last = hist.getSample(1).getVal();
            diff = (double)(cur.getValue()-last.getValue())
                   /(cur.getSampleSec()-lastgetSampleSec());
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
    void State<T>::go(T sp){
        Word<T>* w = new Word<T>(sp, nodeID, 0, SET);
        txDownStream.write(w);
    }

    template <class T>
    State<T>::getHist(){
    }

    template <class T>
    State<T>::diff(){
        
    }

    template <class T>
    void State<T>::assign(Word<T>* w){
        RTT::Logger::log() << RTT::Logger::Debug
                           << "(state: "
                           << th->name << ") Value: " << w->getData();
        hist.update( w->getData() );
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
