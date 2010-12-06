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

#ifndef ACES_TESTSUITE_HPP
#define ACES_TESTSUITE_HPP

#include <string>
#include <sstream>

#include "hardware.hpp"
#include "device.hpp"
#include "protocol.hpp"
#include "state.hpp"
#include "word.hpp"

namespace TestSuite{

    template <typename T>
    class Hardware : public ACES::Hardware<T> {
        public:
            Hardware(std::string cfg, std::string args);
            bool subscribeController(ACES::Controller* c);
    };
    
    template <class S, class P>
    class Device : public ACES::Device<S,P> {
        public:
            Device(std::string cfg, std::string args);
    };

    template <class HW, class P>
    class Protocol : public ACES::Protocol<HW, P>{
        public:
            Protocol(std::string cfg, std::string args);
    };

    class Spinner : public ACES::State<float>{
        public:
            Spinner(std::string cfg, std::string args, bool sampling);
            virtual void sample();
            //virtual void updateHook();
            float amp, dc;
            RTT::os::TimeService::ticks t0;
    };
    
}

#endif
