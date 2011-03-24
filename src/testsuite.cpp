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
    template <class T>
    Hardware<T>::Hardware(std::string cfg, std::string args)
      : ACES::Hardware<T>(cfg, args)
    { }
   
    template <class S, class PD>
    Device<S,PD>::Device(std::string cfg, std::string args)
      : ACES::Device<S,PD>(cfg)
    {
        this->credentials = new ACES::Credentials(args);
    }
    
    template <class HW, class P>
    Protocol<HW,P>::Protocol(std::string cfg, std::string args)
      : ACES::Protocol<HW,P>(cfg, args)
    { }

    Spinner::Spinner(std::string cfg, std::string args, bool sampling, 
                     unsigned int portnum)
     //: ACES::State<float>(cfg, 1, sampling){
     : ACES::State<float>(cfg, 30, sampling, portnum){
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

    //TODO - Total Kludge, there's got to be a _proper_ way to get the compiler
    //to generate the specializations we want and throw them into the library
    void forcegenerate(){
        Device<float, float> d("a", "b");
        Hardware<float> h("a", "b");
        Protocol<float, float> p("e", "f");
    }
};
