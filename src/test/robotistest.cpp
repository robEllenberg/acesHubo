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
    //logger->setLogLevel(RTT::Logger::Info);
    logger->setLogLevel(RTT::Logger::Debug);

    ACES::Dispatcher* d = new ACES::Dispatcher("dispatch");

    RTT::Logger::log() << "Begin Init" << RTT::endlog();

    d->addHardware("hw 90 100","Robotis", "/dev/ttyUSB0"); //"/dev/ttyUSB0");
    d->addProtocol("pro 80 100", "Robotis", "");
    d->linkHP("hw", "pro");

    //Create devices, representing physical nodes on the bus
    d->addDevice("dX 20 50", "Robotis", "2 0.0 1.0");
    d->addDevice("dY 20 50", "Robotis", "3 0.0 1.0");
    d->addDevice("dZ 20 50", "Robotis", "58 0.0 1.0");

    //Add states, each represents 1 property of the device
    d->addState("X 10 50", "Robotis", "");
    d->addState("Y 10 50", "Robotis", "");
    d->addState("Z 10 50", "Robotis", "");

    //Tell the states and devices to pay attention to each other
    d->linkDS("dX", "X");
    d->linkDS("dY", "Y");
    d->linkDS("dZ", "Z");

    //Tell the devices & protocol to pay attention to each other 
    d->linkPD("pro", "dX");
    d->linkPD("pro", "dY");
    d->linkPD("pro", "dZ");

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

    //OCL::FileReporting r("Reporter");
    //r.addPeer(d);

    //RTT::TaskContext tc("progRun");
    //tc.setActivity(new RTT::Activity(5, 0.01, 0, "progRun") );
    //tc.scripting()->loadPrograms("testprog.ops");
    //tc.connectPeers(d);
    //d->start();
    //d->engine()->programs()->getProgram("TestProgram")->start();

    //d->scripting()->loadPrograms("testprog.ops");
    //RTT::plugin::PluginLoader::Instance()->loadService("scripting",d);
    //((RTT::TaskContext*)d)->getProvider<RTT::Scripting>("scripting")
    //                 ->loadPrograms("testprog.ops");
    //d->engine()->programs()->getProgram("TestProgram")->start();
    
    Orocos::TaskBrowser tbrowser(d);
    tbrowser.loop();
    return 0;
}
