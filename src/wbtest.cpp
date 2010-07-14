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

    d->addHardware("wbHW 1 10", "Webots", "");
    d->addProtocol("wbPcol 2 10", "Webots", "");
    d->addState("HY 5 31", "Webots float", "");
    d->addState("LSP 5 31", "Webots float", "");
    d->addState("LSR 5 31", "Webots float", "");
    d->addState("RSP 5 31", "Webots float", "");
    d->addState("RSR 5 31", "Webots float", "");
    d->addState("LHY 5 31", "Webots float", "");
    d->addState("LHR 5 31", "Webots float", "");
    d->addState("LHP 5 31", "Webots float", "");
    d->addState("LKP 5 31", "Webots float", "");
    d->addState("LAP 5 31", "Webots float", "");
    d->addState("LAR 5 31", "Webots float", "");
    d->addState("RHY 5 31", "Webots float", "");
    d->addState("RHR 5 31", "Webots float", "");
    d->addState("RHP 5 31", "Webots float", "");
    d->addState("RKP 5 31", "Webots float", "");
    d->addState("RAP 5 31", "Webots float", "");
    d->addState("RAR 5 31", "Webots float", "");
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

/*
    ACES::Protocol* webot =
             (ACES::Protocol*) new Webots::Protocol(
             (std::string)"wbpcol", (Webots::Hardware*)hw, 3, 50);

    //connectPeers( dispatch);
    //ACES::State* xx = (ACES::State*) new
    //    Webots::State("xx", "Chest_1",
    //    dispatch, 5, 31, 0.0, -1.0 );

    ACES::State* HY = (ACES::State*) new
        //Webots::State("HY", "Waist_1",
        Webots::State("HY", "HY",
        5, 31, 0.0, -1.0 );

    ACES::State* LSP = (ACES::State*) new
        //Webots::State("LSP", "Shoulder_Elbow_2",
        Webots::State("LSP", "LSP",
        5, 31, 0.0, 1.0 );

    ACES::State* LSR = (ACES::State*) new
        //Webots::State("LSR", "Bicep_2",
        Webots::State("LSR", "LSR",
        5, 31, 0.0, 1.0 );

    ACES::State* RSP = (ACES::State*) new
        //Webots::State("RSP", "Shoulder_Elbow_1",
        Webots::State("RSP", "RSP",
        5, 31, 0.0, 1.0 );

    ACES::State* RSR = (ACES::State*) new
        //Webots::State("RSR", "Bicep_1",
        Webots::State("RSR", "RSR",
        5, 31, 0.0, 1.0 );

    ACES::State* LHY = (ACES::State*) new
        //Webots::State("LHY", "Hip_yaw_2",
        Webots::State("LHY", "LHY",
        5, 31, 0.0, 1.0 );

    ACES::State* LHR = (ACES::State*) new
        //Webots::State("LHR", "Hip_Ankle_pitch_roll_2",
        Webots::State("LHR", "LHR",
        5, 31, 0.0, 1.0 );

    ACES::State* LHP = (ACES::State*) new
        //Webots::State("LHP", "Thigh_2",
        Webots::State("LHP", "LHP",
        5, 31, 0.0, -1.0 );

    ACES::State* LKP = (ACES::State*) new
        //Webots::State("LKP", "Shin_2",
        Webots::State("LKP", "LKP",
        5, 31, 0.0, 1.0 );

    ACES::State* LAP = (ACES::State*) new
        //Webots::State("LAP", "Hip_Ankle_pitch_roll_4",
        Webots::State("LAP", "LAP",
        5, 31, 0.0, -1.0 );

    ACES::State* LAR = (ACES::State*) new
        //Webots::State("LAR", "Foot_2",
        Webots::State("LAR", "LAR",
        5, 31, 0.0, 1.0 );

    ACES::State* RHY = (ACES::State*) new
        //Webots::State("RHY", "Hip_yaw_1",
        Webots::State("RHY", "RHY",
        5, 31, 0.0, 1.0 );

    ACES::State* RHR = (ACES::State*) new
        //Webots::State("RHR", "Hip_Ankle_pitch_roll_1",
        Webots::State("RHR", "RHR",
        5, 31, 0.0, -1.0 );

    ACES::State* RHP = (ACES::State*) new
        //Webots::State("RHP", "Thigh_1",
        Webots::State("RHP", "RHP",
        5, 31, 0.0, -1.0 );

    ACES::State* RKP = (ACES::State*) new
        //Webots::State("RKP", "Shin_1",
        Webots::State("RKP", "RKP",
        5, 31, 0.0, 1.0 );

    ACES::State* RAP = (ACES::State*) new
        //Webots::State("RAP", "Hip_Ankle_pitch_roll_3",
        Webots::State("RAP", "RAP",
        5, 31, 0.0, -1.0 );

    ACES::State* RAR = (ACES::State*) new
        //Webots::State("RAR", "Foot_1",
        Webots::State("RAR", "RAR",
        5, 31, 0.0, 1.0 );

    //webot->registerState(xx);
    webot->registerState(HY);
    webot->registerState(LSP);
    webot->registerState(LSR);
    webot->registerState(RSP);
    webot->registerState(RSR);
    webot->registerState(LHY);
    webot->registerState(LHR);
    webot->registerState(LHP);
    webot->registerState(LKP);
    webot->registerState(LAP);
    webot->registerState(LAR);
    webot->registerState(RHY);
    webot->registerState(RHR);
    webot->registerState(RHP);
    webot->registerState(RKP);
    webot->registerState(RAP);
    webot->registerState(RAR);

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

    ACES::WbController* wbctrl = new ACES::WbController("ctrl", pvect,
        "IKscript1.txt", hw, webot, 5, 50);

    //wbctrl->connectPeers( (RTT::TaskContext*) hw);
    //wbctrl->connectPeers( (RTT::TaskContext*) dispatch);

    //hw->start();

*/
    Orocos::TaskBrowser tbrowser(d);
    tbrowser.loop();
    return 0;
}
