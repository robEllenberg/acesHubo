/* AUTORIGHTS
    ACES: Abstracted Controls Engineering Software
    Copyright (C) 2010 Robert Sherbert
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
    please contact the author.
*/

namespace ACES{
    
    /**
     * Filtered state using one of the filter classes
     * @todo Template the code to take standard filters, once a standard is created
     */
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
        filt.update(sp);
        Word<T>* w = new Word<T>(filt.getOutput(), this->nodeID, 0, SET);
        if(not this->samplingAttr){
            this->value = filt.getOutput();
        }
        this->txDownStream.write(w);
    }

    template <class T>
    void FilteredState<T>::assign(Word<T>* w){
        RTT::Logger::log(RTT::Logger::Debug) << "(state: "
                           << this->name << ") Value: "
                           << w->getData() << RTT::endlog();
        //Update the filter, and store the filtered output to the history
        filt.update( w->getData() );
        this->value = filt.getOutput();
        /*
        this->hist.update( this->value );
        if(this->intEnable){
            updateInt(this->hist.getSample(0), this->hist.getSample(1));
        }
        if(this->diffEnable){
            updateDiff(this->hist.getSample(0), this->hist.getSample(1));
        }
        */
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
