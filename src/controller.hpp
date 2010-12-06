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
            virtual bool getStateVector(bool echo=0) = 0;
            virtual bool modStateVector();
            void addCtrl(std::string dest, float sp);
            float checkCtrl(std::string ctrl);
            bool sendCtrl();
            float getSurface(std::string state, std::string attr);

            RTT::OutputPort< std::map<std::string, void*>* > txDownStream;
        protected:
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
            virtual bool getStateVector(bool echo=0);
    };

    class LegCtrl : public ScriptCtrl
    {
        public:
            LegCtrl(std::string config, std::string args);
            virtual bool getStateVector(bool echo=0);
    };

    class ArmCtrl : public ScriptCtrl
    {
        public:
            ArmCtrl(std::string config, std::string args);
            virtual bool getStateVector(bool echo=0);
    };

    class PID : public Controller{
        public:
            PID(std::string config, std::string args);
            virtual bool getStateVector(bool echo=0);
        private:
            std::string inputSurface, outputSurface;
            float Kp, Ki, Kd, val;
    };

    class UserProg : public Controller{
        public:
            UserProg(std::string config, std::string args);
            virtual bool getStateVector(bool echo=0);
            bool startHook();
        private:
            std::string filename;
            std::string progname;
    };

    class UserSM : public Controller{
        public:
            UserSM(std::string config, std::string args);
            virtual bool getStateVector(bool echo=0);
            bool startHook();
        private:
            std::string filename;
            std::string progname;
    };

    class YJCtrl : public LegCtrl {
        public:
            YJCtrl(std::string config, std::string args);
            virtual bool modStateVector();
    };
}

#endif
