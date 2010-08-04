#include "dispatcher.hpp"

namespace ACES{
    Dispatcher::Dispatcher(std::string name)
        : RTT::TaskContext(name),
          addHardwareMethod("addHardware", &Dispatcher::addHardware, this),
          addProtocolMethod("addProtocol", &Dispatcher::addProtocol, this),
          addStateMethod("addState", &Dispatcher::addState, this),
          addControllerMethod("addController", &Dispatcher::addController, this),
          addDeviceMethod("addDevice", &Dispatcher::addDevice, this),
          addLoggerMethod("addLogger", &Dispatcher::addLogger, this),

          startHWMethod("startHW", &Dispatcher::startHW, this),
          startProtocolMethod("startProtocol", &Dispatcher::startProtocol, this),
          startDeviceMethod("startDevice", &Dispatcher::startDevice, this),
          startStateMethod("startState", &Dispatcher::startState, this),
          startControllerMethod("startController", &Dispatcher::startController, this),
          stopHWMethod("stopHW", &Dispatcher::stopHW, this),
          stopProtocolMethod("stopProtocol", &Dispatcher::stopProtocol, this),
          stopDeviceMethod("stopDevice", &Dispatcher::stopDevice, this),
          stopStateMethod("stopState", &Dispatcher::stopState, this),
          stopControllerMethod("stopController", &Dispatcher::stopController, this),
          linkPDMethod("linkPD", &Dispatcher::linkPD, this),
          linkDSMethod("linkDS", &Dispatcher::linkDS, this),
          linkHPMethod("linkHP", &Dispatcher::linkHP, this),
          linkSCMethod("linkSC", &Dispatcher::linkSC, this),
          linkHCMethod("linkHC", &Dispatcher::linkHC, this),
          linkLSMethod("linkLS", &Dispatcher::linkLS, this)
    {
        //this->setActivity( new RTT::Activity(5, 1.0/10.0, 0, name) );
        this->methods()->addMethod(addHardwareMethod, "addHardware",
                                   "config", "_name_ _priority_ _freq_",
                                   "type", "_mainType_ _subType_",
                                   "args", "arguments specific to the type");
        this->methods()->addMethod(addProtocolMethod, "addProtocol",
                                   "config", "_name_ _priority_ _freq_",
                                   "type", "_mainType_ _subType_",
                                   "args", "arguments specific to the type");
        this->methods()->addMethod(addStateMethod, "addState",
                                   "config", "_name_ _priority_ _freq_",
                                   "type", "_mainType_ _subType_",
                                   "args", "arguments specific to the type");
        this->methods()->addMethod(addControllerMethod, "addController",
                                   "config", "_name_ _priority_ _freq_",
                                   "type", "_mainType_ _subType_",
                                   "args", "arguments specific to the type");
        this->methods()->addMethod(addDeviceMethod, "addDevice",
                                   "config", "_name_ _priority_ _freq_",
                                   "type", "_mainType_ _subType_",
                                   "args", "arguments specific to the type");
        this->methods()->addMethod(addLoggerMethod, "addLogger",
                                   "config", "_name_ _priority_ _freq_",
                                   "type", "_mainType_ _subType_",
                                   "args", "arguments specific to the type");

        this->methods()->addMethod(startHWMethod, "startHW");
        this->methods()->addMethod(startProtocolMethod, "startProtocol");
        this->methods()->addMethod(startDeviceMethod, "startDevice");
        this->methods()->addMethod(startStateMethod, "startState");
        this->methods()->addMethod(startControllerMethod, "startController");

        this->methods()->addMethod(stopHWMethod, "stopHW");
        this->methods()->addMethod(stopProtocolMethod, "stopProtocol");
        this->methods()->addMethod(stopDeviceMethod, "stopDevice");
        this->methods()->addMethod(stopStateMethod, "stopState");
        this->methods()->addMethod(stopControllerMethod, "stopController");
                                   
        this->methods()->addMethod(linkPDMethod, "linkPD",
                                   "protocol", "protocol",
                                   "device", "device");
        this->methods()->addMethod(linkDSMethod, "linkDS",
                                   "device", "device",
                                   "state", "state");
        this->methods()->addMethod(linkHPMethod, "linkHP",
                                   "hardware", "hardware",
                                   "protocol", "protocol");
        this->methods()->addMethod(linkSCMethod, "linkSC",
                                   "state", "state",
                                   "controller", "controller");
        this->methods()->addMethod(linkHCMethod, "linkHC",
                                   "hardware", "hardware",
                                   "controller", "controller");
        this->methods()->addMethod(linkLSMethod, "linkLS",
                                   "logger", "logger",
                                   "state", "state");
    
    }
    
    bool Dispatcher::addHardware(std::string cfg, std::string type,
                                 std::string args)
    {
        Hardware* h = NULL;
        
        #ifdef WEBOTS        
        if (type == "Webots") {
            h = (Hardware*) new Webots::Hardware(cfg, args);
        }
        #endif 
        #ifdef TESTSUITE
        if (type == "TestSuite") {
            h = (Hardware*) new TestSuite::Hardware(cfg, args);
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
        Protocol* p = NULL;
        #ifdef WEBOTS
        if ( type == "Webots"){
            p = (Protocol*) new Webots::Protocol(cfg, args);
        } 
        #endif
        #ifdef TESTSUITE
        if ( type == "TestSuite"){
            p = (Protocol*) new TestSuite::Protocol(cfg, args);
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
        Device *d = NULL;
        #ifdef WEBOTS 
        if (type == "Webots"){
            d = (Device*) new Webots::Device(cfg, args);
        }
        #endif
        #ifdef TESTSUITE
        if ( type == "TestSuite"){
            d = (Device*) new TestSuite::Device(cfg, args);
        } 
        #endif
        if(d){
            dList.push_back(d);
            return connectPeers(d);
        }
        return false;
    }

    bool Dispatcher::addState(std::string cfg, std::string type,
                              std::string args)
    {
        //taskCfg c(cfg);
        ProtoState* s = NULL;
        
        if( type == "Webots") {
            s = (ProtoState*) new ACES::State<float>(cfg, args);
            //((State<float>*)s)->printme();
        }

        if( type == "TestSuite") {
            s = (ProtoState*) new ACES::State<float>(cfg, args);
        }
		
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
        #ifdef WEBOTS 
        if ( t1 == "Webots"){
            if (t2 == "Mini"){
                ctrl = (Controller*) new Webots::ScriptCtrl(cfg, args);
            }
            if (t2 == "Null"){
                ctrl = (Controller*) new NullCtrl(cfg, args);
            }
            if (t2 == "Arm"){
                ctrl = (Controller*) new Webots::ArmCtrl(cfg, args);
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
            return ((Protocol*)p)->subscribeDevice((Device*)d);
        }
        else{
            return false;
        }
    }

    bool Dispatcher::linkDS(std::string dev, std::string state){
        RTT::TaskContext* d = getPeer(dev);
        RTT::TaskContext* s = getPeer(state);
        
        if(s && d){
            return ((Device*)d)->subscribeState((ProtoState*)s);
        }
        else{
            return false;
        }
    }

    bool Dispatcher::linkHP(std::string hw, std::string pcol){
        RTT::TaskContext* h = this->getPeer(hw);
        RTT::TaskContext* p = this->getPeer(pcol);

        if (h && p){
            return ((Hardware*)h)->subscribeProtocol((Protocol*)p);
        }
        else{
            return false;
        }
    }

    bool Dispatcher::linkSC(std::string state, std::string ctrl){
        RTT::TaskContext* s = this->getPeer(state);
        RTT::TaskContext* c = this->getPeer(ctrl);

        if (s && c){
            return ((ProtoState*)s)->subscribeController((Controller*)c);
        }
        else{
            return false;
        }
    }

    bool Dispatcher::linkHC(std::string hw, std::string ctrl){
        //Webots::Hardware* h = (Webots::Hardware*)this->getPeer(hw);
		Hardware* h = (Hardware*)this->getPeer(hw);
        RTT::TaskContext* c = this->getPeer(ctrl);
        if (h && c){
            return h->subscribeController((Controller*) c);
        }
        else{
            return false;
        }
    }

    bool Dispatcher::linkLS(std::string logger, std::string state){
        Logger* l = (Logger*)this->getPeer(logger);
        ProtoState* s  = (ProtoState*)this->getPeer(state);
        if(l && s){
            return l->addTrack(state);
        }
    }

    bool Dispatcher::startHW(){
        for(std::list<Hardware*>::iterator it = hwList.begin();
            it != hwList.end(); it++){
            (*it)->start();
        }
        return true;
    }

    bool Dispatcher::startProtocol(){
        for(std::list<Protocol*>::iterator it = pList.begin();
            it != pList.end(); it++){
            (*it)->start();
        }
        return true;
    }

    bool Dispatcher::startDevice(){
        for(std::list<Device*>::iterator it = dList.begin();
            it != dList.end(); it++){
            (*it)->start();
        }
        return true;
    }

    bool Dispatcher::startState(){
        for(std::list<ProtoState*>::iterator it = stateList.begin();
            it != stateList.end(); it++){
            RTT::TaskContext *p = (RTT::TaskContext*)(*it);
            p->start();
        }
        return true;
    }

    bool Dispatcher::startController(){
        for(std::list<Controller*>::iterator it = cList.begin();
            it != cList.end(); it++){
            (*it)->start();
        }
        return true;
    }

    bool Dispatcher::startHook(){
        startHW();
        startProtocol();
        startDevice();
        startState();
        startController();
        //RTT::Logger::log() << "Finished HW" << RTT::endlog();
        //RTT::Logger::log() << "Finished Pcol" << RTT::endlog();
        //RTT::Logger::log() << "Finished States" << RTT::endlog();

        //RTT::Logger::log() << "Finished Controllers" << RTT::endlog();
        return true;
    }
    
    bool Dispatcher::stopController(){
        for(std::list<Controller*>::iterator it = cList.begin();
            it != cList.end(); it++){
            (*it)->stop();
        }
        return true;
    }

    bool Dispatcher::stopState(){
        for(std::list<ProtoState*>::iterator it = stateList.begin();
            it != stateList.end(); it++){
            RTT::TaskContext *p = (RTT::TaskContext*)(*it);
            p->stop();
        }
        return true;
    }

    bool Dispatcher::stopDevice(){
        for(std::list<Device*>::iterator it = dList.begin();
            it != dList.end(); it++){
            (*it)->stop();
        }
        return true;
    }

    bool Dispatcher::stopProtocol(){
        for(std::list<Protocol*>::iterator it = pList.begin();
            it != pList.end(); it++){
            (*it)->stop();
        }
        return true;
    }

    bool Dispatcher::stopHW(){
        for(std::list<Hardware*>::iterator it = hwList.begin();
            it != hwList.end(); it++){
            (*it)->stop();
        }
        return true;
    }

    void Dispatcher::stopHook(){
        stopController();
        stopState();
        stopDevice();
        stopProtocol();
        stopHW();
    }

} 
