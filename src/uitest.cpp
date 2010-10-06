#include <ocl/TaskBrowser.hpp>
//#include <ocl/FileReporting.hpp>
#include <rtt/os/main.h>
#include <rtt/Logger.hpp>
#include <rtt/TaskContext.hpp>

#include "credentials.hpp"
#include "controller.hpp"
#include "message.hpp"
#include "hardware.hpp"
#include "webots.hpp"
#include "dispatcher.hpp"

int ORO_main(int a, char** b){
    RTT::Logger* logger = RTT::Logger::Instance();
    //logger->setLogLevel(RTT::Logger::Info);
    logger->setLogLevel(RTT::Logger::Debug);
    
    RTT::TaskContext launch("launch");
    ACES::Dispatcher d("dispatch");
	launch.connectPeers((RTT::TaskContext*)&d);
        
    //launch.setActivity(new RTT::Activity(5, 1.0/10.0, 0, "launch") );
    //d.scripting()->loadPrograms("testprog.ops");

    launch.start();

    d.scripting()->loadPrograms("launch.ops");

    d.start();
    d.engine()->programs()->getProgram("main")->start();    

    Orocos::TaskBrowser tbrowser(&launch);
    tbrowser.loop();
    //d->start();
    //launch.engine()->programs()->getProgram("TestProgram")->start();
	//while(true){};
	return 0;
}
