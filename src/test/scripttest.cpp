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

    ((RTT::TaskContext*)d)->getProvider<RTT::Scripting>("scripting")
                     ->loadPrograms("testprog.ops");
    d->start();
    
    Orocos::TaskBrowser tbrowser(d);
    tbrowser.loop();
    return 0;
}

