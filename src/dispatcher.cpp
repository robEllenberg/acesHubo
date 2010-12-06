/* AUTORIGHTS
    ACES: Abstracted Controls Engineering Software
    Copyright (C) 2010 Robert Sherbert
    bob.sherbert@gmail.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    If you are interested in licensing this software for commercial purposes
    please contact the author.
*/

#include "dispatcher.hpp"

namespace ACES{
    Dispatcher::Dispatcher(std::string name)
        : RTT::TaskContext(name)
    {
        //Dispatcher must have an activity of some sort or scripts won't run
        //properly
        this->setActivity( new RTT::Activity(5, 1.0/10.0, 0, name) );
        this->addOperation("addHardware", &Dispatcher::addHardware, this,
                           RTT::OwnThread).doc("add new hardware")
                           .arg("config", "_name_ _priority_ _freq_")
                           .arg("type", "_mainType_ _subType_")
                           .arg("args", "arguments specific to the type");
        this->addOperation("addProtocol", &Dispatcher::addProtocol, this,
                           RTT::OwnThread).doc("add new Protocol")
                           .arg("config", "_name_ _priority_ _freq_")
                           .arg("type", "_mainType_ _subType_")
                           .arg("args", "arguments specific to the type");

        this->addOperation("addState", &Dispatcher::addState, this,
                           RTT::OwnThread).doc("add new State")
                           .arg("config", "_name_ _priority_ _freq_")
                           .arg("type", "_mainType_ _subType_")        
                           .arg("args", "arguments specific to the type")
                           .arg("sampling", "Should the state default-sample");

        this->addOperation("addController", &Dispatcher::addController,
                           this, RTT::OwnThread).doc("add new Controller")
                           .arg("config", "_name_ _priority_ _freq_")
                           .arg("type", "_mainType_ _subType_")        
                           .arg("args", "arguments specific to the type");
                           
        this->addOperation("addDevice", &Dispatcher::addDevice, this,
                           RTT::OwnThread).doc("add new Device")
                           .arg("config", "_name_ _priority_ _freq_")
                           .arg("type", "_mainType_ _subType_")
                           .arg("args", "arguments specific to the type");
        this->addOperation("addLogger", &Dispatcher::addLogger, this,
                           RTT::OwnThread).doc("add new Logger")
                           .arg("config", "_name_ _priority_ _freq_")
                           .arg("type", "_mainType_ _subType_")
                           .arg("args", "arguments specific to the type");

        this->addOperation("startDebug", &Dispatcher::startDebug, this,
                           RTT::OwnThread).doc("Start debugging statements");
        this->addOperation("stopDebug", &Dispatcher::stopDebug, this,
                           RTT::OwnThread).doc("Stop debugging statements");

        this->addOperation("startHW", &Dispatcher::startHW, this,
                           RTT::OwnThread).doc("Start Hardware");
        this->addOperation("startProtocol", &Dispatcher::startProtocol,
                           this, RTT::OwnThread).doc("Start Protocol");
        this->addOperation("startDevice", &Dispatcher::startDevice, this,
                           RTT::OwnThread).doc("Start Device");
        this->addOperation("startState", &Dispatcher::startState, this,
                           RTT::OwnThread).doc("Start State");
        this->addOperation("startController", &Dispatcher::startController,
                           this, RTT::OwnThread).doc("Start Controller");
        this->addOperation("startLogger", &Dispatcher::startLogger, this,
                           RTT::OwnThread).doc("Start Logger");

        this->addOperation("stopHW", &Dispatcher::stopHW, this,
                           RTT::OwnThread).doc("Stop Hardware");
        this->addOperation("stopProtocol", &Dispatcher::stopProtocol, this,
                           RTT::OwnThread).doc("Stop Protocol");
        this->addOperation("stopDevice", &Dispatcher::stopDevice, this,
                           RTT::OwnThread).doc("Stop Device");
        this->addOperation("stopState", &Dispatcher::stopState, this,
                           RTT::OwnThread).doc("Stop State");
        this->addOperation("stopController", &Dispatcher::stopController,
                           this, RTT::OwnThread).doc("Stop Controller");
        this->addOperation("stopLogger", &Dispatcher::stopLogger, this,
                           RTT::OwnThread).doc("Stop Logger");
                                   
        this->addOperation("linkPD", &Dispatcher::linkPD, this, RTT::OwnThread)
                           .doc("Link Protocol to Device")
                           .arg("protocol", "protocol")
                           .arg("device", "device");
        this->addOperation("linkDS", &Dispatcher::linkDS, this, RTT::OwnThread)
                           .doc("Link Device to State")
                           .arg("device", "device")
                           .arg("state", "state");
        this->addOperation("linkHP", &Dispatcher::linkHP, this, RTT::OwnThread)
                           .doc("Link Hardware to Protocol")
                           .arg("hardware", "hardware")
                           .arg("protocol", "protocol");
        this->addOperation("linkSC", &Dispatcher::linkSC, this, RTT::OwnThread)
                           .doc("Link State to Controller")
                           .arg("state", "state")
                           .arg("controller", "controller");
        this->addOperation("linkHC", &Dispatcher::linkHC, this, RTT::OwnThread)
                           .doc("Link Hardware to Controller")
                           .arg("hardware", "hardware")
                           .arg("controller", "controller");
        this->addOperation("linkLS", &Dispatcher::linkLS, this, RTT::OwnThread)
                           .doc("Link Logger to State")
                           .arg("logger", "logger")
                           .arg("state", "state");
    }
    
    bool Dispatcher::addHardware(std::string cfg, std::string type,
                                 std::string args)
    {
        RTT::TaskContext* h = NULL;
        
        #ifdef HUBO        
        if (type == "Hubo") {
            h = (RTT::TaskContext*) new Hubo::Hardware(cfg, args);
        }
        #endif 
        #ifdef WEBOTS        
        if (type == "Webots") {
            h = (RTT::TaskContext*) new Webots::Hardware(cfg, args);
        }
        #endif 
        #ifdef ROBOTIS
        if (type == "Robotis") {
            h = (RTT::TaskContext*) new Robotis::Hardware(cfg, args);
        }
        #endif
        #ifdef TESTSUITE
        if (type == "TestSuite") {
            h = (RTT::TaskContext*) new TestSuite::Hardware<float>(cfg, args);
        }
        #endif
        if(h){
            hwList.push_back(h);
            return connectPeers(h);
        }
        return false;
    }

    bool Dispatcher::addProtocol(std::string cfg, std::string type,
                                 std::string args)
    {
        RTT::TaskContext* p = NULL;
        #ifdef HUBO 
        if ( type == "Hubo"){
            p = (RTT::TaskContext*) new Hubo::Protocol(cfg, args);
        } 
        #endif
        #ifdef WEBOTS
        if ( type == "Webots"){
            p = (RTT::TaskContext*) new Webots::Protocol(cfg, args);
        } 
        #endif
        #ifdef ROBOTIS
        if (type == "Robotis"){
            p = (RTT::TaskContext*) new Robotis::Protocol(cfg, args);
        }
        #endif
        #ifdef TESTSUITE
        if ( type == "TestSuite"){
            p = (RTT::TaskContext*) new
                TestSuite::Protocol<float, float>(cfg, args);
        } 
        #endif
        if(p){
            pList.push_back(p);
            return connectPeers(p);
        }
        return false;
    }

    bool Dispatcher::addDevice(std::string cfg, std::string type,
                               std::string args)
    {
        RTT::TaskContext *d = NULL;
        std::istringstream s(type);
        std::string type1, type2;
        s >> type1;
        s >> type2;

        #ifdef HUBO
        if (type1 == "Hubo"){
            d = (RTT::TaskContext*) new Hubo::Device(cfg, args);
        }
        #endif
        #ifdef WEBOTS 
        if (type1 == "Webots"){
            if(type2 == "Joint"){
                d = (RTT::TaskContext*) new Webots::JointDevice(cfg, args);
            }
            if(type2 == "GPS"){
                d = (RTT::TaskContext*) new Webots::GPSDevice(cfg, args);
            }
            if(type2 == "Accelerometer"){
                d = (RTT::TaskContext*) new
                    Webots::AccelerometerDevice(cfg, args);
            }
            if(type2 == "Gyro"){
                d = (RTT::TaskContext*) new
                    Webots::GyroscopeDevice(cfg, args);
            }
            if(type2 == "Force"){
                d = (RTT::TaskContext*) new Webots::ForceDevice(cfg, args);
            }
        }
        #endif
        #ifdef ROBOTIS
        if (type1 == "Robotis"){
            d = (RTT::TaskContext*) new Robotis::Device(cfg, args);
        }
        #endif
        #ifdef TESTSUITE
        if ( type1 == "TestSuite"){
            d = (RTT::TaskContext*) new
                    TestSuite::Device<float,float>(cfg, args);
        } 
        #endif
        if(d){
            dList.push_back(d);
            return connectPeers(d);
        }
        return false;
    }

    bool Dispatcher::addState(std::string cfg, std::string type,
                              std::string args, bool sampling)
    {
        //taskCfg c(cfg);
        ProtoState* s = NULL;
        
        #ifdef HUBO 
        if ( type == "Hubo"){
            s = (ProtoState*) new TestSuite::Spinner(cfg, args, sampling);
        } 
        #endif
        #ifdef WEBOTS
        if( type == "Webots") {
            if ( args == "Joint" ){
                s = (ProtoState*) new ACES::State<float>(cfg, JOINT, sampling);
            }
            if ( args == "X" ) {
                s = (ProtoState*) new ACES::State<float>(cfg, X, sampling);
            }
            if ( args == "Y" ) {
                s = (ProtoState*) new ACES::State<float>(cfg, Y, sampling);
            }
            if ( args == "Z" ) {
                s = (ProtoState*) new ACES::State<float>(cfg, Z, sampling);
            }
            //((State<float>*)s)->printme();
        }
        #endif

        #ifdef ROBOTIS 
        if ( type == "Robotis"){
            s = (ProtoState*) new ACES::State<float>(cfg,
                                                (int)Robotis::GOAL_POSITION,
                                                sampling);
        } 
        #endif

        #ifdef TESTSUITE
        if( type == "TestSuite") {
            s = (ProtoState*) new TestSuite::Spinner(cfg, args, sampling);
        }
        #endif
		
        if(s){
            stateList.push_back(s);
            return connectPeers(s);
        }
        return false;
    }

    bool Dispatcher::addController(std::string cfg, std::string type,
                                   std::string args)
    {
        std::istringstream s1(type);
        //t1 - Major type - Webots, HuboCAN, etc
        //t2 - Minor/data type - int, float, etc
        std::string t1, t2;
        s1 >> t1 >> t2;

        Controller* ctrl = NULL;
        if (t1 == "User"){
            if (t2 == "Program"){
                ctrl = (Controller*) new UserProg(cfg, args);
            }
            if (t2 == "StateMachine"){
                ctrl = (Controller*) new UserSM(cfg, args);
            }
        }
        if (t1 == "PID"){
            ctrl = (Controller*) new PID(cfg, args);
        }
        #ifdef WEBOTS 
        if ( t1 == "Mini"){
            if (t2 == "Leg"){
                ctrl = (Controller*) new LegCtrl(cfg, args);
            }
            if (t2 == "Null"){
                ctrl = (Controller*) new NullCtrl(cfg, args);
            }
            if (t2 == "Arm"){
                ctrl = (Controller*) new ArmCtrl(cfg, args);
            }
            if (t2 == "Youngbum"){
                ctrl = (Controller*) new YJCtrl(cfg, args);
            }
        } 
        #endif
        #ifdef TESTSUITE
        #endif
        if(ctrl){
            cList.push_back(ctrl);
            return connectPeers(ctrl);
        }
        return false;
    }
    
    bool Dispatcher::addLogger(std::string cfg, std::string type,
                               std::string args)
    {
        Logger* log;
        if (type == "File"){
            log = (Logger*) new FileLog(cfg, args);
        }
        if(log){
            logList.push_back(log);
            return connectPeers(log);
        }
        return false;

        /*
        Logger* log;
        if( type == "File"){
            log = (Logger*) new FileLog(cfg, args);
        }

        if(log){
            logList.push_back(log);
            log->subscribeDispatcher(this);
            this->connectPeers(log);
            return true;
        }
        return false;
        */
    }

    bool Dispatcher::linkPD(std::string pcol, std::string device){
        RTT::TaskContext* p = this->getPeer(pcol);
        RTT::TaskContext* d = this->getPeer(device);

        if(p && d){
            return ((ProtoProtocol*)p)->subscribeDevice(d);
        }
        else{
            return false;
        }
    }

    bool Dispatcher::linkDS(std::string dev, std::string state){
        RTT::TaskContext* d = getPeer(dev);
        RTT::TaskContext* s = getPeer(state);
        
        if(s && d){
            return ((ProtoDevice*)d)->subscribeState(s);
        }
        else{
            return false;
        }
    }

    bool Dispatcher::linkHP(std::string hw, std::string pcol){
        RTT::TaskContext* h = this->getPeer(hw);
        RTT::TaskContext* p = this->getPeer(pcol);

        if (h && p){
            return ((ProtoHardware*)h)->subscribeProtocol(p);
        }
        else{
            return false;
        }
    }

    bool Dispatcher::linkSC(std::string state, std::string ctrl){
        RTT::TaskContext* s = this->getPeer(state);
        RTT::TaskContext* c = this->getPeer(ctrl);

        if (s && c){
            return ((ProtoState*)s)->subscribeController(c);
        }
        else{
            return false;
        }
    }

    bool Dispatcher::linkHC(std::string hw, std::string ctrl){
        //Webots::Hardware* h = (Webots::Hardware*)this->getPeer(hw);
		RTT::TaskContext* h = this->getPeer(hw);
        RTT::TaskContext* c = this->getPeer(ctrl);
        if (h && c){
            return ((ProtoHardware*)h)->subscribeController( c);
        }
        else{
            return false;
        }
    }

    bool Dispatcher::linkLS(std::string logger, std::string state, std::string
                            attr)
    {
        RTT::TaskContext* l = this->getPeer(logger);
        RTT::TaskContext* s  = this->getPeer(state);
        if(l && s){
            return ((Logger*)l)->addTrack((ProtoState*)s, attr);
        }
        return false;
    }

    bool Dispatcher::startDebug(){
        RTT::Logger::Instance()->setLogLevel(RTT::Logger::Debug);
        return true;
    }

    bool Dispatcher::stopDebug(){
        RTT::Logger::Instance()->setLogLevel(RTT::Logger::Info);
        return true;
    }

    bool Dispatcher::startHW(){
        for(std::list<RTT::TaskContext*>::iterator it = hwList.begin();
            it != hwList.end(); it++){
            (*it)->start();
        }
        return true;
    }

    bool Dispatcher::startProtocol(){
        for(std::list<RTT::TaskContext*>::iterator it = pList.begin();
            it != pList.end(); it++){
            (*it)->start();
        }
        return true;
    }

    bool Dispatcher::startDevice(){
        for(std::list<RTT::TaskContext*>::iterator it = dList.begin();
            it != dList.end(); it++){
            (*it)->start();
        }
        return true;
    }

    bool Dispatcher::startState(){
        for(std::list<RTT::TaskContext*>::iterator it = stateList.begin();
            it != stateList.end(); it++){
            RTT::TaskContext *p = (RTT::TaskContext*)(*it);
            p->start();
        }
        return true;
    }

    bool Dispatcher::startController(){
        for(std::list<RTT::TaskContext*>::iterator it = cList.begin();
            it != cList.end(); it++){
            (*it)->start();
        }
        return true;
    }

    bool Dispatcher::startLogger(){
        for(std::list<RTT::TaskContext*>::iterator it = logList.begin();
            it != logList.end(); it++){
            (*it)->start();
        }
        return true;
    }


    bool Dispatcher::startHook(){
        //startHW();
        //startProtocol();
        //startDevice();
        //startState();
        //startController();
        //startLogger();

        //RTT::Logger::log() << "Finished HW" << RTT::endlog();
        //RTT::Logger::log() << "Finished Pcol" << RTT::endlog();
        //RTT::Logger::log() << "Finished States" << RTT::endlog();

        //RTT::Logger::log() << "Finished Controllers" << RTT::endlog();
        return true;
    }

    bool Dispatcher::stopLogger(){
        for(std::list<RTT::TaskContext*>::iterator it = logList.begin();
            it != logList.end(); it++){
            (*it)->stop();
        }
        return true;
    }
    
    bool Dispatcher::stopController(){
        for(std::list<RTT::TaskContext*>::iterator it = cList.begin();
            it != cList.end(); it++){
            (*it)->stop();
        }
        return true;
    }

    bool Dispatcher::stopState(){
        for(std::list<RTT::TaskContext*>::iterator it = stateList.begin();
            it != stateList.end(); it++){
            RTT::TaskContext *p = (*it);
            p->stop();
        }
        return true;
    }

    bool Dispatcher::stopDevice(){
        for(std::list<RTT::TaskContext*>::iterator it = dList.begin();
            it != dList.end(); it++){
            (*it)->stop();
        }
        return true;
    }

    bool Dispatcher::stopProtocol(){
        for(std::list<RTT::TaskContext*>::iterator it = pList.begin();
            it != pList.end(); it++){
            (*it)->stop();
        }
        return true;
    }

    bool Dispatcher::stopHW(){
        for(std::list<RTT::TaskContext*>::iterator it = hwList.begin();
            it != hwList.end(); it++){
            (*it)->stop();
        }
        return true;
    }

    void Dispatcher::stopHook(){
        //stopLogger();
        //stopController();
        //stopState();
        //stopDevice();
        //stopProtocol();
        //stopHW();
    }

} 
