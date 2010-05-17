#include <iostream>
#include <string>

#include <ocl/TaskBrowser.hpp>
#include <rtt/os/main.h>
#include <rtt/Logger.hpp>
#include <rtt/Command.hpp>

#include "credentials.hpp"
#include "message.hpp"
#include "hardware.hpp"
#include "controller.hpp"
#include "testsuite.hpp"

int ORO_main(int a, char** b){
    RTT::Logger* logger = RTT::Logger::Instance();
    logger->setLogLevel(RTT::Logger::Debug);

    ACES::Dispatcher dispatch("dispatch", 5, 100);

    TestSuite::Hardware* hw = new 
        TestSuite::Hardware((std::string)"HW", 5, 100);
    hw->start();

    ACES::PValue p(12.3);
    ACES::Credentials c(12);
    ACES::Credentials e(13); 

    TestSuite::Parameter* p1 =
    new TestSuite::Parameter((std::string)"RHR",
                           &c, &dispatch, 5, 10);

    TestSuite::Parameter* p2 =
    new TestSuite::Parameter((std::string)"LHR",
                           &e, &dispatch, 5, 10);

    TestSuite::Protocol proto("wbproto", hw, 5, 50);
    connectPeers( &dispatch, &proto );

    std::vector<ACES::Parameter*> pvect;
    pvect.push_back(p1);
    pvect.push_back(p2);

            
    dispatch.registerParameter(p1, &proto);
    dispatch.registerParameter(p2, &proto);
    dispatch.start();

    ACES::WbController wbctrl("ctrl", pvect,
        "testTools/zmpwalkfast2.txt", 5, 10);
    for(int i = 10000; i > 0; i--){
    }
    wbctrl.start();

    //RTT::Command<bool(ACES::PValue*)> pUpdate =
    // p2->commands()->getCommand<bool(ACES::PValue*)>("setpoint");
    //assert( pUpdate.ready() );
    //pUpdate(&p);

    /*
    std::list<ACES::Credentials*> clist;
    clist.push_back(&c);
    clist.push_back(&d);
    ACES::Message m(clist);
    */
    //ACES::testHardware* hw = new 
    //    ACES::testHardware((std::string)"HW", 5, 2);

/*
    RTT::WriteBufferPort<ACES::Message*> msgprt("msgprt",
                                10);
    msgprt.connectTo(&(hw->inBuffer));
    hw->inBuffer.connectTo(&msgprt);
    //msgprt.Push(&m);
    ACES::WbController(, "testTools/zmpwalkfast2.txt")
    //hw->activate();
    //hw->start();

    //while(1){};
*/
    Orocos::TaskBrowser tbrowser(&dispatch);
    tbrowser.loop();
    return 0;
}
