#include <iostream>
#include <string>

#include <ocl/TaskBrowser.hpp>
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

    d->addHardware("wbHW 1 100", "Webots", "");
    d->addProtocol("wbPcol 2 100", "Webots", "");

    d->addController("wbNull 10 15", "Webots Null", "");
    d->addController("wbctrl 10 15", "Webots Mini", "IKscript1.txt");
    d->addController("wbArm 10 15", "Webots Arm", "sins.txt");

    //d->addDevice("dRSP", "Webots", "RSP 0.0  1.0");
    d->addDevice("dHY", "Webots",  "HY  0.0 -1.0");
    d->addDevice("dLSP", "Webots", "LSP 0.0  1.0");
    d->addDevice("dLSR", "Webots", "LSR 0.0  1.0");
    d->addDevice("dRSP", "Webots", "RSP 0.0  1.0");
    d->addDevice("dRSR", "Webots", "RSR 0.0  1.0");
    d->addDevice("dLHY", "Webots", "LHY 0.0  1.0");
    d->addDevice("dLHR", "Webots", "LHR 0.0  1.0");
    d->addDevice("dLHP", "Webots", "LHP 0.0 -1.0");
    d->addDevice("dLKP", "Webots", "LKP 0.0  1.0");
    d->addDevice("dLAP", "Webots", "LAP 0.0 -1.0");
    d->addDevice("dLAR", "Webots", "LAR 0.0  1.0");
    d->addDevice("dRHY", "Webots", "RHY 0.0  1.0");
    d->addDevice("dRHR", "Webots", "RHR 0.0 -1.0");
    d->addDevice("dRHP", "Webots", "RHP 0.0 -1.0");
    d->addDevice("dRKP", "Webots", "RKP 0.0  1.0");
    d->addDevice("dRAP", "Webots", "RAP 0.0 -1.0");
    d->addDevice("dRAR", "Webots", "RAR 0.0  1.0");

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
    d->linkPD("wbPcol", "dHY"); d->linkDS("dHY",  "HY");
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

    RTT::Logger::log() << "Begin Browser" << RTT::endlog();
    Orocos::TaskBrowser tbrowser(d);
    tbrowser.loop();
    return 0;
}
