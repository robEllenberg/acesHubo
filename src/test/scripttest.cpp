#include <iostream>
#include <string>

#include <ocl/TaskBrowser.hpp>
#include <ocl/FileReporting.hpp>
#include <rtt/os/main.h>
#include <rtt/Logger.hpp>

#include "credentials.hpp"
#include "controller.hpp"
#include "message.hpp"
#include "hardware.hpp"
#include "webots.hpp"
#include "dispatcher.hpp"

int ORO_main(int a, char** b){
    RTT::Logger* logger = RTT::Logger::Instance();
    logger->setLogLevel(RTT::Logger::Info);
    //logger->setLogLevel(RTT::Logger::Debug);

    ACES::Dispatcher* d = new ACES::Dispatcher("dispatch");


    //RTT::TaskContext tc("progRun");
    //tc.setActivity(new RTT::Activity(5, 0.01, 0, "progRun") );
    //tc.scripting()->loadPrograms("testprog.ops");
    //tc.connectPeers(d);

    //d->engine()->programs()->getProgram("TestProgram")->start();
    d->scripting()->loadPrograms("testprog.ops");
    d->start();
    
    Orocos::TaskBrowser tbrowser(d);
    tbrowser.loop();
    return 0;
}

