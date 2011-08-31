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
    template <class HW, class PD>    
    Protocol<HW,PD>::Protocol(std::string cfg, std::string args) :
      ProtoProtocol(cfg,args) 
      //txDownStream("txDownStream"),
      //txUpStream("txUpStream"),
      //dsQueue(10),
      //usQueue(10)
    {
        //requestBuf = new std::deque<Message*>;

        //TODO - Figure out why this flips out when we attempt to declare
        // it as a member instead of a pointer 
        //returnBuf = new RTT::Buffer<ProtoResult*>(250);
        //returnQueue = new std::deque<ProtoResult*>(250);
        //requestQueue = new std::deque<Goal*>(250);
        /*
        this->events()->addEvent(&txDownStream, "txDownStream", "msg",
                                 "The message to be transmitted");
        this->events()->addEvent(&txUpStream, "txUpStream", "result",
                                 "Data struct containing processed result");
        */
        this->ports()->addEventPort("RxDS", rxDownStream).doc(
                               "DownStream (from Device) Reception");
        this->ports()->addEventPort("RxUS", rxUpStream).doc(
                               "UpStream (from Hardware) Reception");
        this->ports()->addPort("TxDS", txDownStream).doc(
                               "DownStream (to Hardware) Transmission");
        this->ports()->addPort("TxUS", txUpStream).doc(
                               "UpStream (to Device) Transmission");
    }

    template <class HW, class PD>    
    void Protocol<HW,PD>::updateHook(){
        Word<HW>* usIn = NULL;
        Word<PD>* usOut = NULL;
        Word<PD>* dsIn = NULL;
        Message<HW>* dsOut = NULL;

        if(rxUpStream.read(usIn) == RTT::NewData){
            do{
                if( (usOut = processUS(usIn)) ){
                    RTT::Logger::log(RTT::Logger::Debug) 
                                       << "(Protocol: "
                                       << name << ") got US" << RTT::endlog();
                    txUpStream.write(usOut);
                }
            }while( rxUpStream.read(usIn) == RTT::NewData );
        }else{
            while( rxDownStream.read(dsIn) == RTT::NewData ){
                if( (dsOut = processDS(dsIn)) ){
                    RTT::Logger::log(RTT::Logger::Debug) 
                                       << "(Protocol: " << name << ") got DS"
                                       << RTT::endlog();
                    txDownStream.write(dsOut);
                }
            }
            txDSPending();
        }

    }

    template <class HW, class PD>    
    Message<HW>* Protocol<HW, PD>::processDS(Word<PD>* p){
        Message<HW>* m = NULL;
        m = new Message<HW>();
        //TODO - VERY VERY BAD
        m->push((Word<HW>*)p);
        return m;
    }

    template <class HW, class PD>    
    Word<PD>* Protocol<HW, PD>::processUS(Word<HW>* h){
        Word<PD>* p = NULL;

        //TODO - Specialized processing function from HWord->PDWord
        //If we're going to implement some kind of state machine or whatever
        //it needs to be done here. This default is only meaningful for 
        //type-same protocols & hardware (not many at all)
        p = (Word<PD>*)h;

        return p;
    }

    template <class HW, class PD>    
    void Protocol<HW, PD>::txDSPending(){
    }
}
