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

#include "hardware.hpp"
namespace ACES {
    ProtoHardware::ProtoHardware(std::string cfg, std::string args) :
      taskCfg(cfg),
      RTT::TaskContext(name)
    {
    }


    bool ProtoHardware::subscribeController(RTT::TaskContext* c){
        return true;
    }

    bool ProtoHardware::subscribeProtocol(RTT::TaskContext* p){
        this->connectPeers(p);
   //     RTT::Handle hand = this->events()->setupConnection("txDownStream")
   //         .callback( h, &Hardware<HW>::rxDownStream
            //           ,p->engine()->events() ).handle();
   //         ).handle(); 
        //RTT::base
        RTT::base::PortInterface *myPort = NULL, *theirPort=NULL;
        bool success;
        RTT::ConnPolicy policy = RTT::ConnPolicy::buffer(200);

        theirPort = (RTT::base::PortInterface*)p->ports()->getPort("RxUS");
        myPort = (RTT::base::PortInterface*)this->ports()->getPort("TxUS");
        success = myPort->connectTo(theirPort, policy);
        if(not success){
            return false;
        }
        
        theirPort = (RTT::base::PortInterface*)p->ports()->getPort("TxDS");
        myPort = (RTT::base::PortInterface*)this->ports()->getPort("RxDS");
        success = theirPort->connectTo(myPort, policy);
        if(not success){
            return false;
        }
                                  
      //  hand = h->events()->setupConnection("txUpStream")
      //      .callback( this, &Protocol<HW,PD>::rxUpStream
            //           ,this->engine()->events() ).handle();
      //      ).handle();
        return true;
    }
    
}
