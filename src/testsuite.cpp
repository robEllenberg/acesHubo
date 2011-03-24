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

#include "testsuite.hpp"

namespace TestSuite{
    Hardware::Hardware(std::string cfg, std::string args)
      : ACES::Hardware<float>(cfg, args)
    { }
   
    Device::Device(std::string cfg, std::string args)
      : ACES::Device<float,float>(cfg)
    {
        this->credentials = new ACES::Credentials(args);
    }
    
    Protocol::Protocol(std::string cfg, std::string args)
      : ACES::Protocol<float,float>(cfg, args)
    { }

    Spinner::Spinner(std::string cfg, std::string args, bool sampling)
     //: ACES::State<float>(cfg, 1, sampling){
     : ACES::State<float>(cfg, 30, sampling){
        std::istringstream s1(args, std::istringstream::in);
        float high, low, amp, dc;
        s1 >> low >> high;
        amp = (high - low) / 2.0;
        dc = (high + low) / 2.0;
        this->amp = amp;
        this->dc = dc;
        t0 = RTT::os::TimeService::Instance()->getTicks();
    }
    
    void Spinner::sample(){
        float t = RTT::os::TimeService::Instance()->secondsSince(t0);
        float val = amp*sin(t) + dc;

        RTT::Logger::log() << RTT::Logger::Debug << "(State: " << name
                           << ") sample: " << val 
                           << RTT::endlog();
        ACES::Word<float> *g = new ACES::Word<float>(val, this->nodeID,
        //                                             0, ACES::REFRESH);
                                                     0, ACES::SET);
        txDownStream.write(g);
    }
};
