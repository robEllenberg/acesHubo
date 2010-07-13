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
      

        if( t1 == "Webots") {
            if (t2 == "float"){
                void* a = Webots::State<float>::parseDispArgs(t2, args);
                void* s =
                     (void*) new Webots::State<float>(c,
                                             (Webots::Credentials<float>*)a,
                                             (float)0.0);
            } 
        }
    }

    bool Dispatcher::addController(std::string cfg, std::string type,
                                   std::string args)
    {}

    bool Dispatcher::configureHook(){
        return true;
    }
   
    bool Dispatcher::startHook(){
        for(std::list<Hardware*>::iterator it = hwList.begin();
            it != hwList.end(); it++){
            (*it)->start();
        }

        for(std::list<Protocol*>::iterator it = pList.begin();
            it != pList.end(); it++){
            (*it)->start();
        }

        for(std::list<void*>::iterator it = stateList.begin();
            it != stateList.end(); it++){
            RTT::TaskContext *p = (RTT::TaskContext*)(*it);
            p->start();
        }

        for(std::list<WbController*>::iterator it = cList.begin();
            it != cList.end(); it++){
            (*it)->start();
        }
        return true;
    }
    
    void Dispatcher::updateHook(){
    }

    void Dispatcher::stopHook(){
        for(std::list<Hardware*>::iterator it = hwList.begin();
            it != hwList.end(); it++){
            (*it)->stop();
        }

        for(std::list<Protocol*>::iterator it = pList.begin();
            it != pList.end(); it++){
            (*it)->stop();
        }

        for(std::list<void*>::iterator it = stateList.begin();
            it != stateList.end(); it++){
            RTT::TaskContext *p = (RTT::TaskContext*)(*it);
            p->stop();
        }

        for(std::list<WbController*>::iterator it = cList.begin();
            it != cList.end(); it++){
            (*it)->stop();
        }
    }

    void Dispatcher::cleanupHook(){
    }
} 
