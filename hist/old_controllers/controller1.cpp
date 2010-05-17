#include "parameter.hpp"
#include "robotis.hpp"
#include "webots.hpp"
#include "hardware.hpp"
#include <rtt/Activity.hpp>
#include <iostream>
#include <fstream>

  int main(){
      //initalize the bus hardware (webots or rs485 bus)
      //initalize two motors
      //demonstrate that both will work in webots and on real hardware
    
      
      //ACES::Webots wb_task("Webots Task");
      //wb_task.setActivity( new RTT::Activity( 5, 0.001 ));
      //wb_task.start();
    
    std::ifstream f("sampleIO/ctrl_tbl_read/read", std::ifstream::in);
    /*
    if(i != 0){
	std::cout << "Error Opening File";
	return 0;
    }*/

    ACES::Hardware rs485port("RS-485 Data Port", &f);
    rs485port.setActivity( new RTT::Activity( 5, 0.001 ));
    Robotis::Robotis dynamixelInterp("Dynamixel Data Interpreter");
    dynamixelInterp.setActivity( new RTT::Activity( 5, 0.01 ));
   
    RTT::Handle h = rs485port.events()->setupConnection("NewData").
	    callback(&dynamixelInterp, &ACES::Protocol::catchData,dynamixelInterp.engine()->events()).handle();
    // catchData(std::string data);
    assert(h.ready());
    h.connect();

    rs485port.outBuffer.connectTo(&dynamixelInterp.inBuffer);

    rs485port.start();
    dynamixelInterp.start();
      for(int i=0; i< 10000; i++){
          for(int j=0; j< 10000; j++);
      }

    rs485port.stop();
    dynamixelInterp.stop();
      return 0;
    /*
	Set up character stream to serial device
	Set up protocol interpreter on character stream
	Create a motor object bound to the protocol interpreter
	    create parameters for each property on the motor
	Send/Recieve data from a motor
	send/recieve data from multiple motors - protocol interpreter should handle mixing
	    
    */
  }
