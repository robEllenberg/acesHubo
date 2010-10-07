#include "../webots.hpp"
#include "../credentials.hpp"

//#include <ocl/TaskBrowser.hpp>
#include <rtt/os/main.h>

using namespace ACES;

int ORO_main(int a, char** b){
    RTT::Logger* logger = RTT::Logger::Instance();
    logger->setLogLevel(RTT::Logger::Debug);
 
    Credentials c(5);
    Credentials d(5);
    Credentials e(6);
    Webots::Credentials w1(GPS, (std::string)"name1");
    Webots::Credentials w2(GPS, (std::string)"name2");
    Webots::Credentials w3(JOINT, (std::string)"name2");
    Webots::Credentials w4(GPS, (std::string)"name1");

    c.printme();
    d.printme();
    bool truth = (c == d);
    bool falsehood = (c == e);
    RTT::Logger::log() << "??? is " << truth << " or "
                       << falsehood << RTT::endlog();
    w1.printme();
    RTT::Logger::log() << (w1 == c)  << RTT::endlog();
    RTT::Logger::log() << (w1 == d)  << RTT::endlog();
    RTT::Logger::log() << (w1 == w2) << RTT::endlog();
    RTT::Logger::log() << (w3 == w2) << RTT::endlog();
    RTT::Logger::log() << (w3 == w1) << RTT::endlog();
    RTT::Logger::log() << (w4 == w1) << RTT::endlog();
    RTT::Logger::log() << (w1 == w4) << RTT::endlog();
    return 0;
}

