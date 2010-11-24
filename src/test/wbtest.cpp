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

    RTT::Logger::log() << "Begin Creation" << RTT::endlog();

    d->addHardware("wbHW 1 600", "Webots", "");
    d->addProtocol("wbPcol 2 600", "Webots", "");

    d->addController("wbNull 10 15", "Mini Null", "");
    //d->addController("wbArm 10 15", "Mini Arm", "sins.txt");

    //Pick one of these three controllers
    //d->addController("wbctrl 10 15", "Mini Leg", "IKscript1.txt");
    d->addController("wbctrl 10 15", "User Program", "miniProg.ops main");
    //d->addController("wbctrl 10 15", "User StateMachine", "miniSM.osd main");

    d->addDevice("dHY 5 31", "Webots Joint",  "HY  0.0 -1.0");
    d->addDevice("dLSP 5 31", "Webots Joint", "LSP 0.0  1.0");
    d->addDevice("dLSR 5 31", "Webots Joint", "LSR 0.0  1.0");
    d->addDevice("dRSP 5 31", "Webots Joint", "RSP 0.0  1.0");
    d->addDevice("dRSR 5 31", "Webots Joint", "RSR 0.0  1.0");
    d->addDevice("dLHY 5 31", "Webots Joint", "LHY 0.0  1.0");
    d->addDevice("dLHR 5 31", "Webots Joint", "LHR 0.0  1.0");
    d->addDevice("dLHP 5 31", "Webots Joint", "LHP 0.0 -1.0");
    d->addDevice("dLKP 5 31", "Webots Joint", "LKP 0.0  1.0");
    d->addDevice("dLAP 5 31", "Webots Joint", "LAP 0.0 -1.0");
    d->addDevice("dLAR 5 31", "Webots Joint", "LAR 0.0  1.0");
    d->addDevice("dRHY 5 31", "Webots Joint", "RHY 0.0  1.0");
    d->addDevice("dRHR 5 31", "Webots Joint", "RHR 0.0 -1.0");
    d->addDevice("dRHP 5 31", "Webots Joint", "RHP 0.0 -1.0");
    d->addDevice("dRKP 5 31", "Webots Joint", "RKP 0.0  1.0");
    d->addDevice("dRAP 5 31", "Webots Joint", "RAP 0.0 -1.0");
    d->addDevice("dRAR 5 31", "Webots Joint", "RAR 0.0  1.0");

    d->addDevice("dGPS 5 31", "Webots GPS", "zero");
    d->addState("centerX 5 5",  "Webots", "X");
    d->addState("centerY 5 5",  "Webots", "Y");
    d->addState("centerZ 5 5",  "Webots", "Z");

    d->linkPD("wbPcol", "dGPS");
    d->linkDS("dGPS", "centerX");
    d->linkDS("dGPS", "centerY");
    d->linkDS("dGPS", "centerZ");

    d->addDevice("dIMUAcc 5 31", "Webots Accelerometer", "imuacc");
    d->addState("imuaccX 5 5",  "Webots", "X");
    d->addState("imuaccY 5 5",  "Webots", "Y");
    d->addState("imuaccZ 5 5",  "Webots", "Z");

    d->linkPD("wbPcol", "dIMUAcc");
    d->linkDS("dIMUAcc", "imuaccX");
    d->linkDS("dIMUAcc", "imuaccY");
    d->linkDS("dIMUAcc", "imuaccZ");

    d->addDevice("dIMUGyro 5 31", "Webots Gyro", "imugyro");
    d->addState("imugyroX 5 5",  "Webots", "X");
    d->addState("imugyroY 5 5",  "Webots", "Y");
    d->addState("imugyroZ 5 5",  "Webots", "Z");

    d->linkPD("wbPcol", "dIMUGyro");
    d->linkDS("dIMUGyro", "imugyroX");
    d->linkDS("dIMUGyro", "imugyroY");
    d->linkDS("dIMUGyro", "imugyroZ");

    d->addDevice("dRFootForce 5 31", "Webots Force", "RFoot");
    d->addState("rffX 5 5",  "Webots", "X");
    d->addState("rffY 5 5",  "Webots", "Y");
    d->addState("rffZ 5 5",  "Webots", "Z");
    d->linkPD("wbPcol", "dRFootForce");
    d->linkDS("dRFootForce", "rffX");
    d->linkDS("dRFootForce", "rffY");
    d->linkDS("dRFootForce", "rffZ");

    d->addDevice("dLFootForce 5 31", "Webots Force", "LFoot");
    d->addState("lffX 5 5",  "Webots", "X");
    d->addState("lffY 5 5",  "Webots", "Y");
    d->addState("lffZ 5 5",  "Webots", "Z");
    d->linkPD("wbPcol", "dLFootForce");
    d->linkDS("dLFootForce", "lffX");
    d->linkDS("dLFootForce", "lffY");
    d->linkDS("dLFootForce", "lffZ");

    d->addState("HY 5 31",  "Webots", "Joint");
    d->addState("LSP 5 31", "Webots", "Joint");
    d->addState("LSR 5 31", "Webots", "Joint");
    d->addState("RSP 5 31", "Webots", "Joint");
    d->addState("RSR 5 31", "Webots", "Joint");
    d->addState("LHY 5 31", "Webots", "Joint");
    d->addState("LHR 5 31", "Webots", "Joint");
    d->addState("LHP 5 31", "Webots", "Joint");
    d->addState("LKP 5 31", "Webots", "Joint");
    d->addState("LAP 5 31", "Webots", "Joint");
    d->addState("LAR 5 31", "Webots", "Joint");
    d->addState("RHY 5 31", "Webots", "Joint");
    d->addState("RHR 5 31", "Webots", "Joint");
    d->addState("RHP 5 31", "Webots", "Joint");
    d->addState("RKP 5 31", "Webots", "Joint");
    d->addState("RAP 5 31", "Webots", "Joint");
    d->addState("RAR 5 31", "Webots", "Joint");

    d->linkDS("dHY",  "HY"); d->linkPD("wbPcol", "dHY");
    d->linkPD("wbPcol", "dLSP"); d->linkDS("dLSP", "LSP");
    d->linkPD("wbPcol", "dLSR"); d->linkDS("dLSR", "LSR");
    d->linkPD("wbPcol", "dRSP"); d->linkDS("dRSP", "RSP");
    d->linkPD("wbPcol", "dRSR"); d->linkDS("dRSR", "RSR");
    d->linkPD("wbPcol", "dLHY"); d->linkDS("dLHY", "LHY");
    d->linkPD("wbPcol", "dLHR"); d->linkDS("dLHR", "LHR");
    d->linkPD("wbPcol", "dLHP"); d->linkDS("dLHP", "LHP");
    d->linkPD("wbPcol", "dLKP"); d->linkDS("dLKP", "LKP");
    d->linkPD("wbPcol", "dLAP"); d->linkDS("dLAP", "LAP");
    d->linkPD("wbPcol", "dLAR"); d->linkDS("dLAR", "LAR");
    d->linkPD("wbPcol", "dRHY"); d->linkDS("dRHY", "RHY");
    d->linkPD("wbPcol", "dRHR"); d->linkDS("dRHR", "RHR");
    d->linkPD("wbPcol", "dRHP"); d->linkDS("dRHP", "RHP");
    d->linkPD("wbPcol", "dRKP"); d->linkDS("dRKP", "RKP");
    d->linkPD("wbPcol", "dRAP"); d->linkDS("dRAP", "RAP");
    d->linkPD("wbPcol", "dRAR"); d->linkDS("dRAR", "RAR");

    d->linkSC("LSP", "wbctrl");
    d->linkSC("LSR", "wbctrl");
    d->linkSC("RSP", "wbctrl");
    d->linkSC("RSR", "wbctrl");
    d->linkSC("HY",  "wbctrl");
    d->linkSC("LHY", "wbctrl");
    d->linkSC("LHR", "wbctrl");
    d->linkSC("LHP", "wbctrl");
    d->linkSC("LKP", "wbctrl");
    d->linkSC("LAP", "wbctrl");
    d->linkSC("LAR", "wbctrl");
    d->linkSC("RHY", "wbctrl");
    d->linkSC("RHR", "wbctrl");
    d->linkSC("RHP", "wbctrl");
    d->linkSC("RKP", "wbctrl");
    d->linkSC("RAP", "wbctrl");
    d->linkSC("RAR", "wbctrl");

    d->linkHC("wbHW", "wbNull");
    d->linkHP("wbHW", "wbPcol");

    d->addLogger("flog 10 5", "File", "tfile.txt");
    
    d->linkLS("flog", "centerX", "value");
    d->linkLS("flog", "centerY", "value");
    d->linkLS("flog", "centerZ", "value");
    //d->linkLS("flog", "imuaccX", "value");
    //d->linkLS("flog", "imuaccY", "value");
    //d->linkLS("flog", "imuaccZ", "value");
    //d->linkLS("flog", "imugyroX", "value");
    //d->linkLS("flog", "imugyroY", "value");
    //d->linkLS("flog", "imugyroZ", "value");
    
    //d->linkLS("flog", "rffX", "value");
    //d->linkLS("flog", "rffY", "value");
    //d->linkLS("flog", "rffZ", "value");
    //d->linkLS("flog", "lffX", "value");
    //d->linkLS("flog", "lffY", "value");
    //d->linkLS("flog", "lffZ", "value");

    //d->linkLS("flog", "LSP", "value");
    //d->linkLS("flog", "LSR", "value");
    //d->linkLS("flog", "RSP", "value");
    //d->linkLS("flog", "RSR", "value");
    //d->linkLS("flog", "HY", "value");
    //d->linkLS("flog", "LHY", "value");
    //d->linkLS("flog", "LHR", "value");
    //d->linkLS("flog", "LHP", "value");
    //d->linkLS("flog", "LKP", "value");
    //d->linkLS("flog", "LAP", "value");
    //d->linkLS("flog", "LAR", "value");
    //d->linkLS("flog", "RHY", "value");
    //d->linkLS("flog", "RHR", "value");
    //d->linkLS("flog", "RHP", "value");
    //d->linkLS("flog", "RKP", "value");
    //d->linkLS("flog", "RAP", "value");
    //d->linkLS("flog", "RAR", "value");

    // END CONFIG SECTION 

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
