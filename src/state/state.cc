/* 
    Conductor: High Degree of Freedom Robot Controller Framework
    Copyright (C) 2010, 2011 Robert Sherbert
    bob.sherbert@gmail.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    If you are interested in licensing this software for commercial purposes
    please contact the author. The software can be licensed to you under
    non-free license terms by the copyright holder.

    As a courtesy to the author, and in the spirit of fair attribution, you are
    asked to cite the following paper if any work based upon or utilizing this
    framework is published in the scientific literature: 
    Sherbert, Robert M. and Oh, Paul Y. "Conductor: A Controller Development
    Framework for High Degree of Freedom Systems." Intelligent Robots and
    Systems (IROS), 2011 IEEE/RSJ International Conference on. 
*/

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
        //hist.pop_back(); //This is no longer necessary with a circular buffer
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
        for(unsigned int i=0; i < hist.size(); i++){
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
    State<T>::State(std::string cfg, int nID, bool sampling, unsigned int
                    portnum) :
      ProtoState(cfg, nID, sampling),
      value(0),
      kInt(1),
      hist(10),
      port(portnum),
      matio(this, port),
      matActivity(priority, &matio)
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
        this->addAttribute("portnum", port);

        this->ports()->addEventPort("RxDS", rxDownStream).doc(
                               "DownStream (from Controller) Reception");
        this->ports()->addEventPort("RxUS", rxUpStream).doc(
                               "UpStream (from Device) Reception");
        this->ports()->addPort("TxDS", txDownStream).doc(
                               "DownStream (to Device) Transmission");

        RTT::ConnPolicy policy = RTT::ConnPolicy::buffer(25);
        matio.DSmatio.connectTo(&rxDownStream, policy);
    }

    template <class T>
    bool State<T>::startHook(){
        matActivity.start();
        return true;
    }

    template <class T>
    void State<T>::stopHook(){
        matActivity.stop();
    }

    template <class T>
    void State<T>::updateHook(){
        std::map<std::string, void*>* dsIn = NULL;
        Word<T>* dsOut = NULL;
        Word<T>* usIn = NULL;

        if( rxDownStream.read(dsIn) == RTT::NewData ){
            do{
                RTT::Logger::log(RTT::Logger::Debug) << "(state: " 
                       << name << ") rxDS" << RTT::endlog();

                if( (dsOut = processDS(dsIn)) ){
                    txDownStream.write(dsOut);
                }
            }
            while(rxDownStream.read(dsIn) == RTT::NewData);
        }
        else if(rxUpStream.read(usIn) == RTT::NewData){
            do{
                RTT::Logger::log(RTT::Logger::Debug) << "(state: " 
                       << name << ") rxUS" << RTT::endlog();
                                   
                if(usIn->getNodeID() == nodeID){
                    RTT::Logger::log(RTT::Logger::Debug) << "(state: "
                                       << name << ") assign"
                                       << RTT::endlog();
                    assign(usIn);
                }
            }while(rxUpStream.read(usIn) == RTT::NewData);
        }
        else if( samplingAttr ){
            sample();
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
        RTT::Logger::log(RTT::Logger::Debug) << "(state: "
                           << name << ") sample"
                           << RTT::endlog();
        Word<T>* s = new Word<T>(0., nodeID, 0, REFRESH);
        txDownStream.write(s);
    }

    template <class T>
    void State<T>::printme(){
        RTT::Logger::log() << "State: " << name
        << ", (Freq:" << freq << ", Pri:"
        << priority << "),"
        << "(Val = " << value.get() << ")" << RTT::endlog();
    }

    template <class T>
    void State<T>::printHistory(){
        hist.printme();
    }

    template <class T>
    void State<T>::go(T sp){
        RTT::Logger::log(RTT::Logger::Debug) << "(state: "
                           << name << ") go: " << (float)sp
                           << RTT::endlog();
        Word<T>* w = new Word<T>(sp, nodeID, 0, SET);
        txDownStream.write(w);
        if(not samplingAttr){
            value = sp;
        }
    }

    template <class T>
    void State<T>::assign(Word<T>* w){
        RTT::Logger::log(RTT::Logger::Debug) << "(state: "
                           << this->name << ") Value: "
                           << w->getData() << RTT::endlog();
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
    T State<T>::getVal(){
        return value;
    }

    template <class T>
    bool State<T>::updateInt(Sample<T> cur, Sample<T> last){
        if(cur.isValid() and last.isValid()){
            double newArea = 1./2.*(double)( cur.getVal()+last.getVal()) *
            (double)( last.getSec() - cur.getSec() );
            integral = newArea + kInt*integral;
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
            go(val);
            //Word<T>* w = new Word<T>(val, nodeID, 0, SET);
            //return w; 
        }
        return NULL;
    }

    template <class T>
    std::string State<T>::logVal(){
        std::stringstream s;
        s << this->value;
        return s.str();
    }

    //FilteredState definitions here
    
    template <class T>
    FilteredState<T>::FilteredState(std::string cfg, int nID, bool sampling, unsigned int  portnum, int fLength) :
            State<T>(cfg, nID, sampling,portnum),
            filt(fLength)
    {
        this->addOperation("setFilterDelay", &FilteredState<T>::setDelay, this, RTT::OwnThread)
            .doc("Assign a delay in samples for the smoothing filter")
            .arg("delay","The time in discrete samples to reach steady-state");
    }
    
    template <class T>
    void FilteredState<T>::go(T sp){
        RTT::Logger::log(RTT::Logger::Debug) << "(state: "
                           << this->name << ") go: " << (float)sp
                           << RTT::endlog();
        Word<T>* w = new Word<T>(sp, this->nodeID, 0, SET);
        this->txDownStream.write(w);
        if(not this->samplingAttr){
            filt.update(sp);
            this->value = filt.getOutput();
        }
    }

    template <class T>
    void FilteredState<T>::assign(Word<T>* w){
        RTT::Logger::log(RTT::Logger::Debug) << "(state: "
                           << this->name << ") Value: "
                           << w->getData() << RTT::endlog();
        //Update the filter, and store the filtered output to the history
        filt.update( w->getData() );
        this->value = filt.getOutput();
        this->hist.update( this->value );
        if(this->intEnable){
            updateInt(this->hist.getSample(0), this->hist.getSample(1));
        }
        if(this->diffEnable){
            updateDiff(this->hist.getSample(0), this->hist.getSample(1));
        }
    }

    template <class T>
    T FilteredState<T>::getVal(){
        return filt.getOutput();
    }

    template <class T>
    void FilteredState<T>::setDelay(int fLength){
        filt.setDelay(fLength);
    }

    //Basic filter
    template <class T>
    ProtoFilter<T>::ProtoFilter(){
        value = 0;
    }

    template <class T>
    T ProtoFilter<T>::getOutput(){
        return value;
    }

    // Smoothing Filter starts here
     
    ///
    /// @brief  Constructor for simple smoothing filter
    /// @param  size of filter (total delay)
    /// @return 
    ///
    template <class T>
    SmoothingFilter<T>::SmoothingFilter( int initSize) : ProtoFilter<T>() {
        oldValue = 0;
        for (int k = 0;k<initSize;k++){
            xBuffer.push_back(0.0);
            vBuffer.push_back(0.0);
            aBuffer.push_back(0.0);
            size=initSize;
        }
    }

    template <class T>
    bool SmoothingFilter<T>::update(T xn){
        //new = previous + (new-oldest)/Length
        // We need the current array size to compute the correct average
        int size = this->size;
        T currentSize = (T)xBuffer.size();
        T vn = 0.0;
        T an = 0.0;
        T newSize = currentSize;
        
        if (size > (int)currentSize){
            newSize = currentSize + 1;
            //Buffer needs to grow to reach new size
            vn = ( vBuffer.front()*currentSize + xn ) / newSize;
            an = ( aBuffer.front()*currentSize + vn ) / newSize;
            this->value = ( oldValue*currentSize + an ) / newSize;
        }
        else{
                //Keep a constant Length
                //
                vn = vBuffer.front() + (xn - xBuffer.back()) / currentSize;
                an = aBuffer.front() + (vn - vBuffer.back()) / currentSize;
                this->value = oldValue + (an - aBuffer.back()) / currentSize;

                //Trim Buffers at back
                trimBuffer(1);
            if (size < (int)currentSize){
                //TODO: Shrinking the buffer currently causes drift, but why?
                //Reproduce this by setting the buffer to something large, with
                //a sinusoidal input, and then cut it down dramatically in
                //script. The shrinking mechanism is theoretically correct, if
                //a little rough in MATLAB.
                newSize = currentSize-1;
                vn = (vn*currentSize - xBuffer.back()) / newSize;
                an = (an*currentSize - vBuffer.back()) / newSize;
                this->value = (this->value*currentSize - aBuffer.back()) / newSize;
                //Trim Buffers at back
                trimBuffer(1);
            }
        }
        //Update stored data for next iteration
        oldValue = this->value;

        xBuffer.push_front(xn);
        vBuffer.push_front(vn);
        aBuffer.push_front(an);

        return true;  //because what could go wrong?
    }

    template <class T>
    void SmoothingFilter<T>::trimBuffer(int count){
        for (int k=0;k<count;k++){
            xBuffer.pop_back();
            vBuffer.pop_back();
            aBuffer.pop_back();
        }
    }

    template <class T>
    bool SmoothingFilter<T>::setDelay(int samples){
        size = sanitizeDelay(samples)/3;
            RTT::Logger::log(RTT::Logger::Warning) << "Filter goal Size: "
                << size <<
                RTT::endlog();
        return true;
    }

    template <class T>
    int SmoothingFilter<T>::sanitizeDelay(int samples){
        //TODO: Make these hardcoded values options, or find a reason to justify them
        if (samples < 6){
            //Minimum delay of filter
            samples = 6;
        }
        if (samples > 200){
            //Maximum delay of filter
            samples = 200;
        }
        return samples;
    }

    template <class T>
    FastFilter<T>::FastFilter( int initSize) : ProtoFilter<T>() {
        oldValue = 0;
        if (initSize<1) initSize=1; //Can't have negative sized filter :)
        xBuffer.resize(initSize,0.0);
        vBuffer.resize(initSize,0.0);
        aBuffer.resize(initSize,0.0);
        size=initSize;
    }

    template <class T>
    bool FastFilter<T>::setDelay (int fLength){
        RTT::Logger::log(RTT::Logger::Warning) << 
            "Cannot change Fastfilter size" <<
            RTT::endlog();
        return false;
    }
    template <class T>
    bool FastFilter<T>::update(T xn){
        //new = previous + (new-oldest)/Length
        // We need the current array size to compute the correct average

        T vn = 0.0;
        T an = 0.0;
        vn = vBuffer.front() + (xn - xBuffer.back()) / size;
        an = aBuffer.front() + (vn - vBuffer.back()) / size;
        this->value = oldValue + (an - aBuffer.back()) / size;

        oldValue = this->value;

        xBuffer.push_front(xn);
        vBuffer.push_front(vn);
        aBuffer.push_front(an);

        return true;  //because what could go wrong?
    }
    
    template <class T>
    DFFilter<T>::DFFilter(std::vector<T> a,std::vector<T> b) : ProtoFilter<T>() {
        xHist.resize(b.size(),0.0);
        yHist.resize(a.size(),0.0);
        this->a(a);
        this->b(b);
    }

    template <class T>
    bool DFFilter<T>::update(T xn){
        typename std::vector<T>::iterator itData;
        typename boost::circular_buffer<T>::iterator itFilter;

        // 1) Push new x data into history
        xHist.push_front(xn);

        // 2) Update yn using this new data 

        T yn = 0.0;

        for (itData=a.begin(),itFilter=xHist.begin();itData<a.end();itData++,itFilter++){
            yn+=((*itData) * (*itFilter));
        }

        // 3) Update the feedback portion of the filter using the new y history
        
        for (itData=b.begin(),itFilter=yHist.begin();itData<b.end();itData++,itFilter++){
            yn+=((*itData) * (*itFilter));
        }

        // 3) Push this new y onto the history
        yHist.push_front(yn);
        this->value = yn;
        return true;  //because what could go wrong?
    }

}
