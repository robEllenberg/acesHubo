#include "dispatcher.hpp"

namespace ACES{
    Dispatcher::Dispatcher(std::string n, int pri, int freq)
        : RTT::TaskContext(n)
    {
        this->setActivity( new RTT::Activity(pri, 1.0/freq, 0, n) );
    }
    
    bool Dispatcher::addHardware(std::string cfg, std::string type,
                                 std::string args)
    {
        taskCfg c(cfg);
        
        Hardware* h;

            if (type == "Webots") {
                h = (Hardware*) new Webots::Hardware(cfg, args);
            } 

        hwList.push_back(h);
        this->connectPeers(h);
    }

    /*  Protocol Args consist of:
     *  the backended hardware
     */ 
    bool Dispatcher::addProtocol(std::string cfg, std::string type,
                                 std::string args)
    {
        taskCfg c(cfg);
        Protocol* p;
        if ( type == "Webots"){
            p = (Protocol*) new Webots::Protocol(cfg, args);
        } 
        if(p){
            pList.push_back(p);
            this->connectPeers(p);
        }
    }

    bool Dispatcher::addDevice(std::string cfg, std::string type,
                               std::string args)
    {
        taskCfg c(cfg);
        std::istringstream s(type);
        std::string t1;
        s >> t1 ;

        Device *d;
        if (type == "Webots"){
            Credentials *cred = Webots::Credentials::parseDispArgs(args);
            d = (Device*) new Webots::Device(c, cred);
        }
        if(d){
            dList.push_back(d);
            this->connectPeers(d);
        }
    }

    bool Dispatcher::addState(std::string cfg, std::string type,
                              std::string args)
    {
        taskCfg c(cfg);
        void* s;
        std::istringstream s1(type);
        //t1 - Major type - Webots, HuboCAN, etc
        //t2 - Minor/data type - int, float, etc
        std::string t1, t2;
        s1 >> t1 >> t2;
      
        void* state;
        if( t1 == "Webots") {
                if( t2 == "joint"){ 
                    state =
                         (void*) new ACES::State<float>(c, Webots::JOINT,
                                                        (float)0.0);
                }
        }
        if(state){
            stateList.push_back(state);
            connectPeers( (RTT::TaskContext*)state);
            return true;
        }
        return false;
    }

    bool Dispatcher::addController(std::string cfg, std::string type,
                                   std::string args)
    {
        taskCfg c(cfg);

        std::istringstream s1(type);
        //t1 - Major type - Webots, HuboCAN, etc
        //t2 - Minor/data type - int, float, etc
        std::string t1, t2;
        s1 >> t1 >> t2;

        WbController* ctrl;
        if ( t1 == "Webots"){
            if (t2 == "Mini"){
                ctrl = (WbController*) new WbController(c, args);
            }
        } 
        if(ctrl){
            cList.push_back(ctrl);
            this->connectPeers(ctrl);
            return true;
        }
        return false;
    }

    bool Dispatcher::linkPD(std::string pcol, std::string device){
        RTT::TaskContext* p = this->getPeer(pcol);
        RTT::TaskContext* d = this->getPeer(device);

        if(p and d){
            return ((Protocol*)p)->subscribeDevice((Device*)d);
        }
        else{
            return false;
        }
    }

    bool Dispatcher::linkDS(std::string dev, std::string state){
        RTT::TaskContext* d = getPeer(dev);
        RTT::TaskContext* s = getPeer(state);
        
        if(s and d){
            return ((Device*)d)->subscribeState((ProtoState*)s);
        }
        else{
            return false;
        }
    }

    bool Dispatcher::linkHP(std::string hw, std::string pcol){
        RTT::TaskContext* h = this->getPeer(hw);
        RTT::TaskContext* p = this->getPeer(pcol);

        if (h and p){
            return ((Hardware*)h)->subscribeProtocol((Protocol*)p);
        }
        else{
            return false;
        }
    }

    bool Dispatcher::linkSC(std::string state, std::string ctrl){
        RTT::TaskContext* s = this->getPeer(state);
        RTT::TaskContext* c = this->getPeer(ctrl);

        if (s and c){
            return ((ProtoState*)s)->subscribeController((WbController*)c);
        }
        else{
            return false;
        }
    }

    bool Dispatcher::linkHC(std::string hw, std::string ctrl){
        Webots::Hardware* h = (Webots::Hardware*)this->getPeer(hw);
        RTT::TaskContext* c = this->getPeer(ctrl);
        if (h and c){
            return h->subscribeController((ACES::WbController*) c);
        }
        else{
            return false;
        }
    }

    bool Dispatcher::configureHook(){
        return true;
    }
   
    bool Dispatcher::startHook(){
        for(std::list<Hardware*>::iterator it = hwList.begin();
            it != hwList.end(); it++){
            (*it)->start();
        }

        //RTT::Logger::log() << "Finished HW" << RTT::endlog();

        for(std::list<Protocol*>::iterator it = pList.begin();
            it != pList.end(); it++){
            (*it)->start();
        }

        for(std::list<Device*>::iterator it = dList.begin();
            it != dList.end(); it++){
            (*it)->start();
        }

        //RTT::Logger::log() << "Finished Pcol" << RTT::endlog();
        for(std::list<void*>::iterator it = stateList.begin();
            it != stateList.end(); it++){
            RTT::TaskContext *p = (RTT::TaskContext*)(*it);
            p->start();
        }

        //RTT::Logger::log() << "Finished States" << RTT::endlog();

        for(std::list<WbController*>::iterator it = cList.begin();
            it != cList.end(); it++){
            (*it)->start();
        }

        //RTT::Logger::log() << "Finished Controllers" << RTT::endlog();
        return true;
    }
    
    void Dispatcher::updateHook(){
    }

    void Dispatcher::stopHook(){
        for(std::list<WbController*>::iterator it = cList.begin();
            it != cList.end(); it++){
            (*it)->stop();
        }

        for(std::list<void*>::iterator it = stateList.begin();
            it != stateList.end(); it++){
            RTT::TaskContext *p = (RTT::TaskContext*)(*it);
            p->stop();
        }

        for(std::list<Device*>::iterator it = dList.begin();
            it != dList.end(); it++){
            (*it)->stop();
        }

        for(std::list<Protocol*>::iterator it = pList.begin();
            it != pList.end(); it++){
            (*it)->stop();
        }

        for(std::list<Hardware*>::iterator it = hwList.begin();
            it != hwList.end(); it++){
            (*it)->stop();
        }
    }

    void Dispatcher::cleanupHook(){
    }
} 
