#include <iostream>
#include <string>

#include <ocl/TaskBrowser.hpp>
#include <ocl/FileReporting.hpp>
#include <rtt/os/main.h>
#include <rtt/Logger.hpp>
#include <rtt/TaskContext.hpp>
#include <rtt/Activity.hpp>
#include <rtt/scripting/Scripting.hpp>
//#include <rtt/scripting/ScriptingService.hpp>
//#include <rtt/plugin/PluginLoader.hpp>

//#include "credentials.hpp"
//#include "controller.hpp"
//#include "message.hpp"
//#include "hardware.hpp"
#include "../dispatcher.hpp"
//#include "webots.hpp"

int ORO_main(int a, char** b){
    RTT::Logger* logger = RTT::Logger::Instance();
    logger->setLogLevel(RTT::Logger::Info);
    //logger->setLogLevel(RTT::Logger::Debug);

    ACES::Dispatcher* d = new ACES::Dispatcher("dispatch");

    RTT::Logger::log() << "Begin Init" << RTT::endlog();

    d->addHardware("tsHW 90 100","TestSuite","");
    d->addProtocol("tsPro 80 100", "TestSuite", "");
    d->linkHP("tsHW", "tsPro");

    //Create devices, representing physical nodes on the bus
    d->addDevice("dX 20 50", "TestSuite", "1");
    d->addDevice("dY 20 50", "TestSuite", "2");
    d->addDevice("dZ 20 50", "TestSuite", "3");

    //Add states, each represents 1 property of the device
    d->addState("X 10 50", "TestSuite", "1.0 2.0");
    d->addState("Y 10 50", "TestSuite", "3.0 4.0");
    d->addState("Z 10 50", "TestSuite", "5.0 6.0");

    //Tell the states and devices to pay attention to each other
    d->linkDS("dX", "X");
    d->linkDS("dY", "Y");
    d->linkDS("dZ", "Z");

    //Tell the devices & protocol to pay attention to each other 
    d->linkPD("tsPro", "dX");
    d->linkPD("tsPro", "dY");
    d->linkPD("tsPro", "dZ");

    //Setup Logging
    d->addLogger("log1 5 200", "File", "testlog.txt");
    d->linkLS("log1", "X");
    d->linkLS("log1", "Y");
    d->linkLS("log1", "Z");
    
    /* END CONFIG SECTION */

    //Start the threads
    RTT::Logger::log() << "Begin Browser" << RTT::endlog();
    d->start();
    d->startHW();
    d->startProtocol();
    d->startDevice();
    d->startState();
    d->startController();
    d->startLogger();

    //((RTT::TaskContext*)d)->getProvider<RTT::Scripting>("scripting")
    //                 ->loadPrograms("testsuiteprog.ops");

    Orocos::TaskBrowser tbrowser(d);
    tbrowser.loop();
    return 0;
}
