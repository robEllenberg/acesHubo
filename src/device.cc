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
    template <class S, class PD>
    Device<S,PD>::Device(std::string config) :
      ProtoDevice(config)
    {
        /*this->events()->addEvent(&txDownStream, "txDownStream", "goal",
                                 "The Goal/SP Data");
        this->events()->addEvent(&txUpStream, "txUpStream", "data",
                                 "Interperted data going to states");*/
        this->addOperation("credentials", &Device<S,PD>::printCred, this,
                           RTT::OwnThread).doc("Print the Credentials");
        this->ports()->addPort("RxDS", rxDownStream).doc(
                               "DownStream (from State) Reception");
        this->ports()->addPort("RxUS", rxUpStream).doc(
                               "UpStream (from Protocol) Reception");
        this->ports()->addPort("TxDS", txDownStream).doc(
                               "DownStream (to Protocol) Transmission");
        this->ports()->addPort("TxUS", txUpStream).doc(
                               "UpStream (to State) Transmission");
        this->setActivity(
            new RTT::Activity( priority, 1.0/freq, 0, name )
        );
    }

    template <class S, class PD>
    void Device<S,PD>::updateHook(){
        Word<S>* dsIn = NULL;
        Word<PD>* dsOut = NULL;
        if(not DSlockout){
            while( rxDownStream.read(dsIn) == RTT::NewData ){
                if( dsOut = processDS(dsIn) ){
                    RTT::Logger::log() << RTT::Logger::Debug << "(dev: " 
                                       << name << ") got DS"
                                       << RTT::endlog();
                    txDownStream.write(dsOut);
                }
            }
        }
        
        Word<PD>* usIn = NULL;
        Word<S>* usOut = NULL;
        if(not USlockout){
            while( rxUpStream.read(usIn) == RTT::NewData ){
                if( usOut = processUS(usIn) ){
                    RTT::Logger::log() << RTT::Logger::Debug << "(dev: "
                                       << name << ") got US"
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
    }

    template <class S, class PD>
    Word<S>* Device<S,PD>::processUS(Word<PD>* usIn){
        Word<S>* usOut = NULL;
        if(*(usIn->getCred()) == *credentials){
            //Only works on equiv types
            //TODO - VERY VERY BAD
            return (Word<S>*)usIn; 
        }
        return NULL;
    }

    template <class S, class PD>
    Word<PD>* Device<S,PD>::processDS(Word<S>* dsIn){
        Word<PD>* dsOut = NULL;
        dsIn->setCred(credentials);
        //Only works on equiv types
        //TODO - VERY VERY BAD
        return (Word<PD>*)dsIn; 
    }

    template <class S, class P>
    void Device<S,P>::printCred(){
        credentials->printme();
    }

}
