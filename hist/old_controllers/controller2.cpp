#include "parameter.hpp"
#include "robotis.hpp"
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
      //initalize the bus hardware (webots or rs485 bus)
      //initalize two motors
      //demonstrate that both will work in webots and on real hardware
    RTT::Logger* logger = RTT::Logger::Instance();
    logger->setLogLevel(RTT::Logger::Debug);
    
    //std::fstream f("sampleIO/ctrl_tbl_read/read", std::ifstream::in);
    //std::fstream f("call.io", std::fstream::in | std::fstream::out | std::fstream::app);
    //std::fstream f("/dev/ttyUSB0", std::fstream::in | std::fstream::out | std::fstream::app);
    //std::fstream f("/dev/ttyUSB0", std::fstream::in | std::fstream::out);
    std::ifstream fin("/dev/ttyUSB0");
    std::ofstream fout("/dev/ttyUSB0");
    //if (!f.is_open()){
    //    while(1){
    //    }
    //}
    
    Robotis::RobotisHardware rs485port("RS-485 Data Port", &fin, &fout, 2, 300);
    
    std::string* R = new std::string("Robotis");  
    ACES::Dispatcher* dispatch = new ACES::Dispatcher(std::string("Dispatcher"), 1, 25);
    
    Robotis::RobotisProtocol* robotis = new Robotis::RobotisProtocol("Protocol", &rs485port, 2, 300);
    
    connectPeers( dispatch, robotis );
    
    ACES::Credentials* cred12 = new ACES::Credentials(12);
    ACES::Credentials* cred13 = new ACES::Credentials(13);
    ACES::Credentials* cred14 = new ACES::Credentials(14);
    ACES::Credentials* cred15 = new ACES::Credentials(15);    
    ACES::Credentials* cred16 = new ACES::Credentials(16);    
    ACES::Credentials* cred17 = new ACES::Credentials(17);    
    Robotis::RobotisParameter* p12 = new Robotis::RobotisParameter(
        (std::string)"FP", cred12, dispatch, 5, 10);
    Robotis::RobotisParameter* p13 = new Robotis::RobotisParameter(
        (std::string)"FR", cred13, dispatch, 5, 10);
    Robotis::RobotisParameter* p14 = new Robotis::RobotisParameter(
        (std::string)"KL", cred14, dispatch, 5, 10);
    Robotis::RobotisParameter* p15 = new Robotis::RobotisParameter(
        (std::string)"HR", cred15, dispatch, 5, 10);
    Robotis::RobotisParameter* p16 = new Robotis::RobotisParameter(
        (std::string)"HP", cred16, dispatch, 5, 10);
    Robotis::RobotisParameter* p17 = new Robotis::RobotisParameter(
        (std::string)"KU", cred17, dispatch, 5, 10);
        
    (*dispatch).registerParameter(p12, robotis);
    (*dispatch).registerParameter(p13, robotis);
    (*dispatch).registerParameter(p14, robotis);
    (*dispatch).registerParameter(p15, robotis);
    (*dispatch).registerParameter(p16, robotis);
    (*dispatch).registerParameter(p17, robotis);
    
    ACES::Webots* webot = new ACES::Webots((std::string)"Webots", 10, 32);
    robotis->attachWebots(webot);
    webot->start();

    (*dispatch).activate();
    
    (*dispatch).start();
    while(1){}
     Orocos::TaskBrowser tbrowser(dispatch);
     tbrowser.loop();
     return 0;
    
    // while(true){
    //    (*p1).printme();
    //    (*p2).printme();
    // }
  }
