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
    logger->setLogLevel(RTT::Logger::Debug);

    ACES::Dispatcher* d = new ACES::Dispatcher("dispatch");

    RTT::Logger::log() << "Begin Creation" << RTT::endlog();

    d->addHardware("wbHW 1 600", "Webots", "");
    d->addProtocol("wbPcol 2 600", "Webots", "");

    d->addController("wbNull 10 15", "Webots Null", "");
    d->addController("wbctrl 10 15", "Webots Mini", "IKscript1.txt");
    d->addController("wbArm 10 15", "Webots Arm", "sins.txt");

    //d->addDevice("dRSP 5 31", "Webots", "RSP 0.0  1.0");
    d->addDevice("dHY 5 31", "Webots",  "HY  0.0 -1.0");
    d->addDevice("dLSP 5 31", "Webots", "LSP 0.0  1.0");
    d->addDevice("dLSR 5 31", "Webots", "LSR 0.0  1.0");
    d->addDevice("dRSP 5 31", "Webots", "RSP 0.0  1.0");
    d->addDevice("dRSR 5 31", "Webots", "RSR 0.0  1.0");
    d->addDevice("dLHY 5 31", "Webots", "LHY 0.0  1.0");
    d->addDevice("dLHR 5 31", "Webots", "LHR 0.0  1.0");
    d->addDevice("dLHP 5 31", "Webots", "LHP 0.0 -1.0");
    d->addDevice("dLKP 5 31", "Webots", "LKP 0.0  1.0");
    d->addDevice("dLAP 5 31", "Webots", "LAP 0.0 -1.0");
    d->addDevice("dLAR 5 31", "Webots", "LAR 0.0  1.0");
    d->addDevice("dRHY 5 31", "Webots", "RHY 0.0  1.0");
    d->addDevice("dRHR 5 31", "Webots", "RHR 0.0 -1.0");
    d->addDevice("dRHP 5 31", "Webots", "RHP 0.0 -1.0");
    d->addDevice("dRKP 5 31", "Webots", "RKP 0.0  1.0");
    d->addDevice("dRAP 5 31", "Webots", "RAP 0.0 -1.0");
    d->addDevice("dRAR 5 31", "Webots", "RAR 0.0  1.0");

    //d->addState("RSP 5 31", "Webots", "Joint");
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

    //d->linkPD("wbPcol", "dRSP"); d->linkDS("dRSP", "RSP");
    //d->linkSC("RSP", "wbArm");
    RTT::Logger::log() << "Begin Link" << RTT::endlog();
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

    d->linkSC("LSP", "wbArm");
    d->linkSC("LSR", "wbArm");
    d->linkSC("RSP", "wbArm");
    d->linkSC("RSR", "wbArm");
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
    //d->linkLS("flog", "RSP");

    RTT::Logger::log() << "Begin Browser" << RTT::endlog();

    OCL::FileReporting r("Reporter");
    r.addPeer(d);

    //RTT::TaskContext tc("progRun");
    //tc.setActivity(new RTT::Activity(5, 0.01, 0, "progRun") );
    //tc.scripting()->loadPrograms("testprog.ops");
    //tc.connectPeers(d);
    //d->start();
    //d->engine()->programs()->getProgram("TestProgram")->start();

    d->scripting()->loadPrograms("testprog.ops");
    d->start();
    //d->engine()->programs()->getProgram("TestProgram")->start();
    Orocos::TaskBrowser tbrowser(d);
    tbrowser.loop();
    return 0;
}
