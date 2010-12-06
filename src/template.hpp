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

#ifndef ACES_TEMPLATE_HPP
#define ACES_TEMPLATE_HPP

#include "hardware.hpp"
#include "protocol.hpp"
#include "device.hpp"
//#include "state.hpp" //If you need to derrive the state (try to avoid this)
#include "credentials.hpp"
//#include "word.hpp" // Word/Result
//#include "message.hpp"

/* If you need to implement your own logging functions. (you shouldn't) */
//#include "logger.hpp" 

namespace Template{

    class Hardware : public ACES::Hardware {
        public:
            Hardware(std::string cfg, std::string args);
            virtual bool txBus(Message* m);
            virtual bool rxBus();
            virtual Message* processDSQueue();
            virtual ProtoWord* processUSQueue();
            virtual bool subscribeController(ACES::Controller* c);
    };
 
    class Protocol : public ACES::Protocol{
        public:
            Protocol(std::string cfg, std::string args);
            virtual void rxDownStream(Goal*);
            virtual void rxUpStream(ProtoWord*);
            virtual Message* processDSQueue();
            virtual ProtoResult* processUSQueue();
    };
   
    class Device : public ACES::Device {
        public:
            Device(std::string cfg, std::string args);
            virtual void rxUpStream(ProtoResult* rx);
            virtual void rxDownStream(Goal* g);
            virtual Goal* processDSQueue();
            virtual ProtoResult* processUSQueue();
    };
    
}



#endif
