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

#include "state.hpp"

namespace ACES {
    ProtoState::ProtoState(std::string cfg, int nID, bool sampling) :
      ACESTask(cfg),
      nodeID(nID),
      //Switching this default to false prevents automatic
      //sampling from starting
      samplingAttr(sampling)
      //intEnable(true),
      //diffEnable(true),
      //diffThreshold(10e-6)
    {
        //nodeID.set(nID);
        //samplingAttr.set(true);
        this->addAttribute("NodeID", nodeID);
        this->addAttribute("sampling", samplingAttr);
        //this->addAttribute("intEnable", intEnable);
        //this->addAttribute("diffEnable", diffEnable);
        //this->addAttribute("diffThreshold", diffThreshold);
    }

    bool ProtoState::subscribeController(RTT::TaskContext* c){
        this->connectPeers( (RTT::TaskContext*) c);

        RTT::base::PortInterface *myPort = NULL, *theirPort=NULL;
        bool success;
        RTT::ConnPolicy policy = RTT::ConnPolicy::buffer(200);
        
        theirPort = (RTT::base::PortInterface*)c->ports()->getPort("TxDS");
        myPort = (RTT::base::PortInterface*)this->ports()->getPort("RxDS");
        success = theirPort->connectTo(myPort, policy);
        if(not success){
            return false;
        }

        /*RTT::Handle h = c->events()->setupConnection("applyStateVector")
                .callback( this, &State<T>::rxDownStream
                          , c->engine()->events()
                        ).handle();*/
        return true;
    }

    const std::string& ProtoState::getName(){
        return name;
    }
   
}
