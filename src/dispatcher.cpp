#include "dispatcher.hpp"

namespace ACES{
    Dispatcher::Dispatcher(){}
    
    }

    bool addHardware(std::string cfg, std::string type, std::string args){
        taskCfg c(cfg);
        
        Hardware h;
        switch(type){
            case "Webots" :{
                h = (Hardware*) new Webots::Hardware(cfg, args);
            } break;
            defult:
                break;
        }
        hwList.push_back(h);
    }

    /*  Protocol Args consist of:
     *  the backended hardware
     */ 
    bool addProtocol(std::string cfg, std::string type, std::string args){
        taskCfg c(cfg);
        Protocol p;
        switch(type){
            case "Webots" :{
                p = (Protocol*) new Webots::Protocol(cfg, args);
            } break;
        }
    }

    bool addState(std::string cfg, std::string type, std::string args){
        taskCfg c(cfg);
        void* s;
        std::istringstream s1(type);
        std::string t1, t2;
        s1 >> t1 >> t2;
      
        switch(t1){
            case "Webots" : {
                switch(t2){
                    case "float" : {
                        s = (void*) Webots::State<float>(cfg, , 0.0)
                    } break;
                    default:
                        break;
                }
            } break;

            default:
                break;
        }

    }

    bool addController(std::string cfg, std::string type, std::string args){

    }

    taskCfg::taskCfg(std::string cfg){
        std::istringstream s1(cfg);
        s1 >> name;
        s1 >> pri;
        s1 >> freq;
    }
    
    Dispatcher::startHook(){
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
    }

    Dispatcher::stopHook(){
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
