#include <iostream>
#include <string>

#include <ocl/TaskBrowser.hpp>
#include <ocl/FileReporting.hpp>
#include <rtt/os/main.h>
#include <rtt/Logger.hpp>

#include "../credentials.hpp"
#include "../controller.hpp"
#include "../message.hpp"
#include "../hardware.hpp"
#include "../webots.hpp"
#include "../dispatcher.hpp"

int ORO_main(int a, char** b){
    RTT::Logger* logger = RTT::Logger::Instance();
    logger->setLogLevel(RTT::Logger::Info);
    //logger->setLogLevel(RTT::Logger::Debug);

    ACES::Dispatcher* d = new ACES::Dispatcher("dispatch");
    d->addHardware("wbHW 1 600", "Webots", "");
    d->addProtocol("wbPcol 2 600", "Webots", "");

    d->addDevice("dHIP 5 31", "Webots Joint",  "hip 0.0 1.0");
    d->addDevice("dhzs 5 31", "Webots Joint", "horizontal_servo 0.0 1.0");
 
    d->addState("hip 5 31",  "Webots", "Joint");
    d->addState("hzs 5 31",  "Webots", "Joint");

    d->addController("wbNull 10 15", "Mini Null", "");
    d->addController("user 10 30", "User StateMachine", "smscript.osd main");

    d->addLogger("flog 10 5", "File", "tfile.txt");

    d->linkHP("wbHW", "wbPcol");
    d->linkPD("wbPcol", "dHIP");
    d->linkPD("wbPcol", "dhzs");
    d->linkDS("dHIP", "hip");
    d->linkDS("dhzs", "hzs");

    d->linkSC("hip", "user");
    d->linkSC("hzs", "user");
    d->linkHC("wbHW", "wbNull");

    d->linkLS("flog", "hip", "value");
    d->linkLS("flog", "hzs", "value");

    //Start the threads
    d->startHW();
    d->startProtocol();
    d->startDevice();
    d->startState();
    d->startLogger();
    d->startController();
    /* IMPORTANT STUFF HERE */

    d->start();
    //d->engine()->programs()->getProgram("TestProgram")->start();
    
    Orocos::TaskBrowser tbrowser(d);
    tbrowser.loop();
    return 0;
}
