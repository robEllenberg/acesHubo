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

#include "hardware.hpp"
namespace ACES {
    ProtoHardware::ProtoHardware(std::string cfg, std::string args) :
      ACESTask(cfg)
    {
        this->ports()->addPort("packetReport", packetReporter).doc(
               "Port to report transmission of setpoints to controller");
    }

    bool ProtoHardware::subscribeController(RTT::TaskContext* c){
        this->connectPeers(c);
        RTT::base::PortInterface *myPort = NULL, *theirPort=NULL;
        bool success;
        RTT::ConnPolicy policy = RTT::ConnPolicy::buffer(200);
        theirPort = (RTT::base::PortInterface*)c->ports()->getPort("packetReporter");
        myPort = (RTT::base::PortInterface*)this->ports()->getPort("packetReporter");
        success = myPort->connectTo(theirPort, policy);
        assert(success);
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
 
    charHardware::charHardware(std::string cfg, std::string args)
      : Hardware<unsigned char>(cfg, args),
        io_service(),
        port(io_service, (const char*)args.c_str())
    {}

    bool charHardware::txBus(ACES::Message<unsigned char>* m){
        if(m){
            ACES::Word<unsigned char>* w = NULL;
            std::vector<unsigned char> buf;
            while(m->size()){
                w = m->pop();
                buf.push_back(w->getData());
            }
            for(std::vector<unsigned char>::iterator it = buf.begin();
                it != buf.end(); it++)
            {
                port.write_some(boost::asio::buffer((void*)(&(*it)), 1));
                RTT::Logger::log() << (int) *it  << RTT::endlog();
            }
            return true;
        }
        return false;
    }

    pStreamHardware::pStreamHardware(std::string cfg, std::string args)
      : Hardware<unsigned char>(cfg, args),
        ioFD( open( (const char*)args.c_str(), O_RDWR) )//,
        //io_service(),
        //port(io_service, ::dup(STDOUT_FILENO))
        //::dup(ioFD)
    {}

    bool pStreamHardware::txBus(ACES::Message<unsigned char>* m){
        if(m){
            ACES::Word<unsigned char>* w = NULL;
            std::vector<unsigned char> buf;
            while(m->size()){
                w = m->pop();
                buf.push_back(w->getData());
            }
            for(std::vector<unsigned char>::iterator it = buf.begin();
                it != buf.end(); it++)
            {
                //port.write_some(boost::asio::buffer((void*)(&(*it)), 1));
                char c = *it;
                write(ioFD, &c, 1);
                RTT::Logger::log() << (int) *it  << RTT::endlog();
            }
            return true;
        }
        return false;
    }
}
