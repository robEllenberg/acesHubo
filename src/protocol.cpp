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

#include "protocol.hpp"

namespace ACES {
    ProtoProtocol::ProtoProtocol(std::string cfg, std::string args) :
      taskCfg(cfg),
      RTT::TaskContext(name)
    {}

    bool ProtoProtocol::subscribeDevice(RTT::TaskContext* d){
        this->connectPeers( (RTT::TaskContext*) d );

        RTT::base::PortInterface *myPort = NULL, *theirPort=NULL;
        bool success;
        RTT::ConnPolicy policy = RTT::ConnPolicy::buffer(200);
        
        theirPort = (RTT::base::PortInterface*)d->ports()->getPort("RxUS");
        myPort = (RTT::base::PortInterface*)this->ports()->getPort("TxUS");
        success = myPort->connectTo(theirPort, policy);
        if(not success){
            return false;
        }

        theirPort = (RTT::base::PortInterface*)d->ports()->getPort("TxDS");
        myPort = (RTT::base::PortInterface*)this->ports()->getPort("RxDS");
        success = theirPort->connectTo(myPort, policy);
        if(not success){
            return false;
        }

        /*RTT::Handle h = d->events()->setupConnection("txDownStream")
            .callback( this, &Protocol<HW,PD>::rxDownStream,
                       d->engine()->events()
                     ).handle();*/
        
        /*h = this->events()->setupConnection("txUpStream")
            .callback( d, &Device<S, PD>::rxUpStream
        //               ,this->engine()->events()
                     ).handle();*/
 
        return true;
    }
}

