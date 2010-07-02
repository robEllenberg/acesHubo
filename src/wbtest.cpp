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


int ORO_main(int a, char** b){
    RTT::Logger* logger = RTT::Logger::Instance();
    logger->setLogLevel(RTT::Logger::Debug);

    ACES::Dispatcher* dispatch = new ACES::Dispatcher(
        "dispatch", 5, 100
    );

    ACES::Hardware* hw = (ACES::Hardware*) new 
        Webots::Hardware((std::string)"HW", 10, 50);

    ACES::Protocol* webot =
             (ACES::Protocol*) new Webots::Protocol(
             (std::string)"wbpcol", (Webots::Hardware*)hw, 3, 50);

    connectPeers( dispatch, webot);
    //ACES::Parameter* xx = (ACES::Parameter*) new
    //    Webots::Parameter("xx", "Chest_1",
    //    dispatch, 5, 31, 0.0, -1.0 );

    ACES::Parameter* HY = (ACES::Parameter*) new
        //Webots::Parameter("HY", "Waist_1",
        Webots::Parameter("HY", "HY",
        dispatch, 5, 31, 0.0, -1.0 );

    ACES::Parameter* LSP = (ACES::Parameter*) new
        //Webots::Parameter("LSP", "Shoulder_Elbow_2",
        Webots::Parameter("LSP", "LSP",
        dispatch, 5, 31, 0.0, 1.0 );

    ACES::Parameter* LSR = (ACES::Parameter*) new
        //Webots::Parameter("LSR", "Bicep_2",
        Webots::Parameter("LSR", "LSR",
        dispatch, 5, 31, 0.0, 1.0 );

    ACES::Parameter* RSP = (ACES::Parameter*) new
        //Webots::Parameter("RSP", "Shoulder_Elbow_1",
        Webots::Parameter("RSP", "RSP",
        dispatch, 5, 31, 0.0, 1.0 );

    ACES::Parameter* RSR = (ACES::Parameter*) new
        //Webots::Parameter("RSR", "Bicep_1",
        Webots::Parameter("RSR", "RSR",
        dispatch, 5, 31, 0.0, 1.0 );

    ACES::Parameter* LHY = (ACES::Parameter*) new
        //Webots::Parameter("LHY", "Hip_yaw_2",
        Webots::Parameter("LHY", "LHY",
        dispatch, 5, 31, 0.0, 1.0 );

    ACES::Parameter* LHR = (ACES::Parameter*) new
        //Webots::Parameter("LHR", "Hip_Ankle_pitch_roll_2",
        Webots::Parameter("LHR", "LHR",
        dispatch, 5, 31, 0.0, 1.0 );

    ACES::Parameter* LHP = (ACES::Parameter*) new
        //Webots::Parameter("LHP", "Thigh_2",
        Webots::Parameter("LHP", "LHP",
        dispatch, 5, 31, 0.0, -1.0 );

    ACES::Parameter* LKP = (ACES::Parameter*) new
        //Webots::Parameter("LKP", "Shin_2",
        Webots::Parameter("LKP", "LKP",
        dispatch, 5, 31, 0.0, 1.0 );

    ACES::Parameter* LAP = (ACES::Parameter*) new
        //Webots::Parameter("LAP", "Hip_Ankle_pitch_roll_4",
        Webots::Parameter("LAP", "LAP",
        dispatch, 5, 31, 0.0, -1.0 );

    ACES::Parameter* LAR = (ACES::Parameter*) new
        //Webots::Parameter("LAR", "Foot_2",
        Webots::Parameter("LAR", "LAR",
        dispatch, 5, 31, 0.0, 1.0 );

    ACES::Parameter* RHY = (ACES::Parameter*) new
        //Webots::Parameter("RHY", "Hip_yaw_1",
        Webots::Parameter("RHY", "RHY",
        dispatch, 5, 31, 0.0, 1.0 );

    ACES::Parameter* RHR = (ACES::Parameter*) new
        //Webots::Parameter("RHR", "Hip_Ankle_pitch_roll_1",
        Webots::Parameter("RHR", "RHR",
        dispatch, 5, 31, 0.0, -1.0 );

    ACES::Parameter* RHP = (ACES::Parameter*) new
        //Webots::Parameter("RHP", "Thigh_1",
        Webots::Parameter("RHP", "RHP",
        dispatch, 5, 31, 0.0, -1.0 );

    ACES::Parameter* RKP = (ACES::Parameter*) new
        //Webots::Parameter("RKP", "Shin_1",
        Webots::Parameter("RKP", "RKP",
        dispatch, 5, 31, 0.0, 1.0 );

    ACES::Parameter* RAP = (ACES::Parameter*) new
        //Webots::Parameter("RAP", "Hip_Ankle_pitch_roll_3",
        Webots::Parameter("RAP", "RAP",
        dispatch, 5, 31, 0.0, -1.0 );

    ACES::Parameter* RAR = (ACES::Parameter*) new
        //Webots::Parameter("RAR", "Foot_1",
        Webots::Parameter("RAR", "RAR",
        dispatch, 5, 31, 0.0, 1.0 );

    //dispatch->registerParameter(xx, webot);
    dispatch->registerParameter(HY, webot);
    dispatch->registerParameter(LSP, webot);
    dispatch->registerParameter(LSR, webot);
    dispatch->registerParameter(RSP, webot);
    dispatch->registerParameter(RSR, webot);
    dispatch->registerParameter(LHY, webot);
    dispatch->registerParameter(LHR, webot);
    dispatch->registerParameter(LHP, webot);
    dispatch->registerParameter(LKP, webot);
    dispatch->registerParameter(LAP, webot);
    dispatch->registerParameter(LAR, webot);
    dispatch->registerParameter(RHY, webot);
    dispatch->registerParameter(RHR, webot);
    dispatch->registerParameter(RHP, webot);
    dispatch->registerParameter(RKP, webot);
    dispatch->registerParameter(RAP, webot);
    dispatch->registerParameter(RAR, webot);

    std::vector<ACES::Parameter*> pvect;
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
        "IKscript1.txt", hw, dispatch, 5, 50);

    wbctrl->connectPeers( (RTT::TaskContext*) hw);
    wbctrl->connectPeers( (RTT::TaskContext*) dispatch);

    //hw->start();
    //dispatch->start();
    //wbctrl.start();

    Orocos::TaskBrowser tbrowser(wbctrl);
    tbrowser.loop();
    return 0;
}
