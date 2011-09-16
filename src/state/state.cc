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
    History<T>::History(int s) : hist(s)
    {
        //TODO: eliminate the size field since it can't be changed anyway
        size = s;
        //this->hist.resize(s);
        //Added this to initialize the buffer properly
        lastValid = -1;
    }

    template <class T>
    History<T>::History(int s, Sample<T> ic) : hist(s)
    {
        size = s;
        //Added this to initialize the buffer properly
        //this->hist.resize(s);
        hist[0] = ic;
        lastValid = 0;
    }
    
    template <class T>
    void History<T>::update(T value)
    {
        Sample<T> s(value, RTT::os::TimeService::Instance()->getTicks());
        hist.push_front(s); 
        hist.pop_back(); //This is no longer necessary with a circular buffer
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

    
    /**
     * Basic State.
     * @param cfg String of configuration information, typically "name priority freq"
     * @param nID Node number of state (when it eventually connects to a device)
     */
    template <class T>
    State<T>::State(std::string cfg, int nID, bool sampling, unsigned int
                    portnum) :
      ProtoState(cfg, nID, sampling),
      value(0),
      //kInt(1),
      //hist(10),
      port(portnum),
      matio(this, port),
      matActivity(priority, &matio)
    {
        this->addOperation("sample", &State<T>::sample, this, RTT::OwnThread
                          ).doc("Sample the State");
        this->addOperation("go", &State<T>::go, this, RTT::OwnThread).doc(
                           "Go to a specified Setpoint").arg("SP", "Set Point");
        //this->addOperation("printHist", &State<T>::printHistory, this,
        //                   RTT::OwnThread).doc("Print the history");

        this->addAttribute("value", value);
        //this->addAttribute("integral", integral);
        //this->addAttribute("diff", diff);
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

    /*
    template <class T>
    float State<T>::getInt(){
        return integral;
    }

    template <class T>
    float State<T>::getDiff(){
        return diff;
    }
    */

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

/*
    template <class T>
    void State<T>::printHistory(){
        hist.printme();
    }
*/

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
        value = w->getData();

        /*
         * hist.update( w->getData() );
        if(intEnable){
            updateInt(hist.getSample(0), hist.getSample(1));
        }
        if(diffEnable){
            updateDiff(hist.getSample(0), hist.getSample(1));
        }
        */

    }

    template <class T>
    T State<T>::getVal(){
        return value;
    }

/*
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
*/
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

}
