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
