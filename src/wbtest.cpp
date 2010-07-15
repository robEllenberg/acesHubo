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

    ACES::Dispatcher* d = new ACES::Dispatcher("dispatch", 20, 1);

/*
    ACES::State<float>* s1 = new ACES::State<float>("s1", 0, 5, 31, 0.0);
    ACES::Protocol* p1 = new ACES::Protocol("p1", 3, 50);
    RTT::Handle h = s1->events()->setupConnection("announceGoal").
                        callback( p1, &ACES::Protocol::addRequest).handle();
    assert(h.ready());
    h.connect();
    assert(h.connected());
    p1->subscribeState(s1);
    ACES::Dispatcher(p1, s1);
*/

    RTT::Logger::log() << "Begin Creation" << RTT::endlog();

    d->addHardware("wbHW 1 40", "Webots", "");
    d->addProtocol("wbPcol 2 40", "Webots", "");
    d->addController("wbctrl 3 15", "Webots Mini", "IKscript1.txt");

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

    d->addState("HY 5 31",  "Webots joint", "");
    d->addState("LSP 5 31", "Webots joint", "");
    d->addState("LSR 5 31", "Webots joint", "");
    d->addState("RSP 5 31", "Webots joint", "");
    d->addState("RSR 5 31", "Webots joint", "");
    d->addState("LHY 5 31", "Webots joint", "");
    d->addState("LHR 5 31", "Webots joint", "");
    d->addState("LHP 5 31", "Webots joint", "");
    d->addState("LKP 5 31", "Webots joint", "");
    d->addState("LAP 5 31", "Webots joint", "");
    d->addState("LAR 5 31", "Webots joint", "");
    d->addState("RHY 5 31", "Webots joint", "");
    d->addState("RHR 5 31", "Webots joint", "");
    d->addState("RHP 5 31", "Webots joint", "");
    d->addState("RKP 5 31", "Webots joint", "");
    d->addState("RAP 5 31", "Webots joint", "");
    d->addState("RAR 5 31", "Webots joint", "");

    RTT::Logger::log() << "Begin Link" << RTT::endlog();
    d->linkPS("wbPcol", "HY");
    d->linkPS("wbPcol", "LSP");
    d->linkPS("wbPcol", "LSR");
    d->linkPS("wbPcol", "RSP");
    d->linkPS("wbPcol", "RSR");
    d->linkPS("wbPcol", "LHY");
    d->linkPS("wbPcol", "LHR");
    d->linkPS("wbPcol", "LHP");
    d->linkPS("wbPcol", "LKP");
    d->linkPS("wbPcol", "LAP");
    d->linkPS("wbPcol", "LAR");
    d->linkPS("wbPcol", "RHY");
    d->linkPS("wbPcol", "RHR");
    d->linkPS("wbPcol", "RHP");
    d->linkPS("wbPcol", "RKP");
    d->linkPS("wbPcol", "RAP");
    d->linkPS("wbPcol", "RAR");

    d->linkSC("HY", "wbctrl");
    d->linkSC("LSP", "wbctrl");
    d->linkSC("LSR", "wbctrl");
    d->linkSC("RSP", "wbctrl");
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

    d->linkHC("wbHW", "wbctrl");
    d->linkHP("wbHW", "wbPcol");

/*
    std::vector<ACES::State*> pvect;
    //pvect.push_back(xx);
    pvect.push_back(HY);
    pvect.push_back(LSP);
    pvect.push_back(LSR);
    pvect.push_back(RSP);
    pvect.push_back(RSR);
    pvect.push_back(LHY);
    pvect.push_back(LHR);
    pvect.push_back(LHP);
    pvect.push_back(LKP);
    pvect.push_back(LAP);
    pvect.push_back(LAR);
    pvect.push_back(RHY);
    pvect.push_back(RHR);
    pvect.push_back(RHP);
    pvect.push_back(RKP);
    pvect.push_back(RAP);
    pvect.push_back(RAR);

*/

    RTT::Logger::log() << "Begin Browser" << RTT::endlog();
    Orocos::TaskBrowser tbrowser(d);
    tbrowser.loop();
    return 0;
}
