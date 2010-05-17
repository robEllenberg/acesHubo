#include "parameter.hpp"
#include "webots.hpp"
#include "hardware.hpp"
#include <ocl/TaskBrowser.hpp>
#include <rtt/os/main.h>
#include <rtt/Activity.hpp>
#include <rtt/Logger.hpp>

#include <iostream>
#include <fstream>
#include  <string>

  int ORO_main(int a, char** b){
    RTT::Logger* logger = RTT::Logger::Instance();
    logger->setLogLevel(RTT::Logger::Debug);
    
    //std::ifstream fin("/dev/ttyUSB0");
    //std::ofstream fout("/dev/ttyUSB0");
   
    //Robotis::RobotisHardware rs485port("RS-485 Data Port",
    //                                 &fin, &fout, 2, 300);

    Webots::Hardware webotHW((std::string)"webotHW", 2, 15);
    
    ACES::Dispatcher* dispatch =
      new ACES::Dispatcher(std::string("Dispatcher"), 1, 25);

    ACES::Protocol* webot = Webots::Protocol(
             (std::string)"wbpcol", &webotHW, 3, 100);
    
    ACES::Credentials* global =
      (ACES::Credentials*)new Webots::Credentials("Chest_1");
    ACES::Credentials* CenterHipYaw =
      (ACES::Credentials*)new Webots::Credentials("Waist_1");

    ACES::Credentials* LeftSholderPitch =
      (ACES::Credentials*)new
         Webots::Credentials("Shoulder_Elbow_2");
    ACES::Credentials* LeftSholderRoll =
      (ACES::Credentials*)new Webots::Credentials("Bicep_2");

    ACES::Credentials* RightSholderPitch =
      (ACES::Credentials*)new
         Webots::Credentials("Shoulder_Elbow_1");
    ACES::Credentials* RightSholderRoll =
      (ACES::Credentials*)new Webots::Credentials("Bicep_1");

    ACES::Credentials* LeftHipYaw =
      (ACES::Credentials*)new
         Webots::Credentials("Hip_yaw_2");
    ACES::Credentials* LeftHipRoll =
      (ACES::Credentials*)new
         Webots::Credentials("Hip_Ankle_pitch_roll_2");
    ACES::Credentials* LeftHipPitch =
      (ACES::Credentials*)new Webots::Credentials("Thigh_2");
    ACES::Credentials* LeftKneePitch =
      (ACES::Credentials*)new Webots::Credentials("Shin_2");
    ACES::Credentials* LeftAnklePitch =
      (ACES::Credentials*)new
         Webots::Credentials("Hip_Ankle_pitch_roll_4");
    ACES::Credentials* LeftAnkleRoll =
      (ACES::Credentials*)new Webots::Credentials("Foot_2");

    ACES::Credentials* RightHipYaw =
      (ACES::Credentials*)new
         Webots::Credentials("Hip_yaw_1");
    ACES::Credentials* RightHipRoll =
      (ACES::Credentials*)new
         Webots::Credentials("Hip_Ankle_pitch_roll_1");
    ACES::Credentials* RightHipPitch =
      (ACES::Credentials*)new Webots::Credentials("Thigh_1");
    ACES::Credentials* RightKneePitch =
      (ACES::Credentials*)new Webots::Credentials("Shin_1");
    ACES::Credentials* RightAnklePitch =
      (ACES::Credentials*)new
         Webots::Credentials("Hip_Ankle_pitch_roll_3");
    ACES::Credentials* RightAnkleRoll =
      (ACES::Credentials*)new Webots::Credentials("Foot_1");

    Webots::Parameter* GLO = new Webots::Parameter(
           "Global", global, dispatch, 5, 10);
    Webots::Parameter* CHY = new Webots::Parameter(
           "Global", CenterHipYaw, dispatch, 5, 10);
    Webots::Parameter* LSP = new Webots::Parameter(
           "Global", LeftSholderPitch, dispatch, 5, 10);
       
    (*dispatch).registerParameter(p12, webot);
    (*dispatch).registerParameter(p13, webot);
    (*dispatch).registerParameter(p14, webot);

    (*dispatch).activate();
    
    (*dispatch).start();

     Orocos::TaskBrowser tbrowser(dispatch);
     tbrowser.loop();
    while(1){}
     return 0;
    
    // while(true){
    //    (*p1).printme();
    //    (*p2).printme();
    // }
  }
