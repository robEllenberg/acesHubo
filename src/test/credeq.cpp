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

#include "../webots.hpp"
#include "../credentials.hpp"

//#include <ocl/TaskBrowser.hpp>
#include <rtt/os/main.h>

using namespace ACES;

int ORO_main(int a, char** b){
    RTT::Logger* logger = RTT::Logger::Instance();
    logger->setLogLevel(RTT::Logger::Debug);
 
    Credentials c(5);
    Credentials d(5);
    Credentials e(6);
    Webots::Credentials w1(GPS, (std::string)"name1");
    Webots::Credentials w2(GPS, (std::string)"name2");
    Webots::Credentials w3(JOINT, (std::string)"name2");
    Webots::Credentials w4(GPS, (std::string)"name1");

    c.printme();
    d.printme();
    bool truth = (c == d);
    bool falsehood = (c == e);
    RTT::Logger::log() << "??? is " << truth << " or "
                       << falsehood << RTT::endlog();
    w1.printme();
    RTT::Logger::log() << (w1 == c)  << RTT::endlog();
    RTT::Logger::log() << (w1 == d)  << RTT::endlog();
    RTT::Logger::log() << (w1 == w2) << RTT::endlog();
    RTT::Logger::log() << (w3 == w2) << RTT::endlog();
    RTT::Logger::log() << (w3 == w1) << RTT::endlog();
    RTT::Logger::log() << (w4 == w1) << RTT::endlog();
    RTT::Logger::log() << (w1 == w4) << RTT::endlog();
    return 0;
}

