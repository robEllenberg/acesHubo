#ifndef ACES_CONTROLLER_HPP
#define ACES_CONTROLLER_HPP

#include <vector>
#include <map>
#include <list>
#include <iostream>
#include <fstream>
#include <string>

#include <rtt/TaskContext.hpp>
#include <rtt/Activity.hpp>
#include <rtt/InputPort.hpp>
#include <rtt/OutputPort.hpp>
#include <rtt/scripting/Scripting.hpp>

#include "taskcfg.hpp"

//TODO - Relocate this definition 
enum WB_CTRL_STATES { WB_CTRL_HALT, WB_CTRL_RUN, WB_CTRL_STEP };

namespace ACES{
    class Controller :  protected taskCfg,
                        public RTT::TaskContext
    {
        public:
            Controller(std::string cfg, std::string args);
            virtual void updateHook(); 
            virtual std::map<std::string, void*>* 
                    getStateVector(bool echo=0) = 0;
            void addCtrl(std::string dest, float sp);
            bool sendCtrl();
            float getSurface(std::string state, std::string attr);

            RTT::OutputPort< std::map<std::string, void*>* > txDownStream;
        private:
            std::map<std::string, void*>* curMap;
    };

    class ScriptCtrl : public Controller
    {
        public:
            ScriptCtrl(std::string cfg, std::string args);
            void updateHook();

            void step();
            bool run();
            void halt();
            bool openScript(std::string scriptPath);

            std::ifstream walkScript;
            int simState;
    };

    class NullCtrl : public Controller 
    {
        public:
            NullCtrl(std::string config, std::string args);
            void updateHook();

            std::map<std::string, void*>* 
                    getStateVector(bool echo=0);
    };

    class LegCtrl : public ScriptCtrl
    {
        public:
            LegCtrl(std::string config, std::string args);

            std::map<std::string, void*>* 
                    getStateVector(bool echo=0);
    };

    class ArmCtrl : public ScriptCtrl
    {
        public:
            ArmCtrl(std::string config, std::string args);
            std::map<std::string, void*>* 
                    getStateVector(bool echo=0);
    };

    class PID : public Controller{
        public:
            PID(std::string config, std::string args);
            std::map<std::string, void*>* getStateVector(bool echo=0);
                    
        private:
            std::string inputSurface, outputSurface;
            float Kp, Ki, Kd, val;
    };

    class UserProg : public Controller{
        public:
            UserProg(std::string config, std::string args);
            virtual std::map<std::string, void*>* getStateVector(bool echo=0);
            bool startHook();
        private:
            std::string filename;
            std::string progname;
    };
    class UserSM : public Controller{
        public:
            UserSM(std::string config, std::string args);
            virtual std::map<std::string, void*>* getStateVector(bool echo=0);
            bool startHook();
        private:
            std::string filename;
            std::string progname;
    };
}

#endif
