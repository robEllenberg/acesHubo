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

namespace ACES {
    template <typename T>
    Hardware<T>::Hardware(std::string cfg, std::string args) :
        ProtoHardware(cfg, args),
        //txUpStream("txUpStream"),
        //dsQueue(10),
        usQueue(10)
    {
        //this->events()->addEvent(&txUpStream, "txUpStream", "word",
        //                         "Recieved Data");

        this->ports()->addPort("TxUS", txUpStream).doc(
                               "UpStream (to Protocol) Transmission");
        this->ports()->addEventPort("RxDS", rxDownStream).doc(
                               "DownStream (from Protocol) Reception");

    }

    template <typename T>
    bool Hardware<T>::processUS(Word<T>* usIn){
        return true;
    }

    template <class T>
    bool Hardware<T>::processDS(Message<T>* dsIn){
        return true;
    }

    template <class T>
    void Hardware<T>::updateHook(){
    
        Message<T>* dsIn = NULL;
        while( rxDownStream.read(dsIn) == RTT::NewData ){
            //dsIn->printme();
            if( processDS(dsIn) ){
                RTT::Logger::log(RTT::Logger::Debug) << "(HW: " << name 
                                   << ") got DS"
                                   << RTT::endlog();
                txBus(dsIn);
                //reportTransmission(dsIn);
            }
        }
        rxBus();
        Word<T>* usIn = NULL;
        //while( rxUpStream.read(usIn) == RTT::NewData ){
        
        while(not usQueue.isEmpty() ){
            usQueue.dequeue(usIn);
            if( processUS(usIn) ){
                RTT::Logger::log(RTT::Logger::Debug) << "(HW: " 
                                   << name << ") got US"
                                   << RTT::endlog();
                txUpStream.write(usIn);
            }
        }
        
    }

    template<class T>
    void Hardware<T>::reportTransmission(Message<T>* m){
        //Word<T>* w = m->peek();
        //if(w->getMode() == ACES::SET){
            packetReporter.write(1);
        //}
    }

    template <class T>
    bool Hardware<T>::txBus(Message<T>* m){
        while( m->size() ){
            Word<T>* w = m->pop();
            txUpStream.write(w);
            //usQueue.enqueue(w);
            //TODO - Delete the word
        }
        return true;
    }

    template <class T>
    void Hardware<T>::rxBus(int size){
    }

}
