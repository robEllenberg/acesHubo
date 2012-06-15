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

#include <iostream>
#include <string>

#include <ocl/TaskBrowser.hpp>
#include <rtt/os/main.h>
#include <rtt/Logger.hpp>

#include "dispatcher.hpp"

int ORO_main(int argc, char** argv){
    //RTT::Logger* logger = RTT::Logger::Instance();
    //logger->setLogLevel(RTT::Logger::Info);
    //RTT::Logger::Instance()->setLogLevel(RTT::Logger::Debug);

    //RTT::Logger::log() << "argc " << argc << RTT::endlog();
    std::string fname, progname;
    if(argc == 2){
        fname = argv[1];
        //RTT::Logger::log() << "filename " << fname << RTT::endlog();
        progname = "main";
    }
    else if(argc == 3){
        fname = argv[1];
        progname = argv[2];
    }
    else{
        fname = "dispatcher.ops";
        progname = "main";
    }

    ACES::Dispatcher* d = new ACES::Dispatcher("dispatch");
    Orocos::TaskBrowser tbrowser(d);

    //RTT::Logger::log(RTT::Logger::Info) << "Reading File: " << fname << RTT::endlog();
    //RTT::Logger::log(RTT::Logger::Info) << "Reading Program: " << progname << RTT::endlog();

    d->start();
    d->getProvider<RTT::Scripting>("scripting")->loadPrograms(fname);
    for(int i = 0; i <32000; i++){
        for(int j = 0; j < 32000; j++)
            {}
    }
    d->getProvider<RTT::Scripting>("scripting")->startProgram(progname);

    tbrowser.loop();
    return 0;
}
