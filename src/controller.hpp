/* 
    Conductor: High Degree of Freedom Robot Controller Framework
    Copyright (C) 2010, 2011 Robert Sherbert
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
    please contact the author. The software can be licensed to you under
    non-free license terms by the copyright holder.

    As a courtesy to the author, and in the spirit of fair attribution, you are
    asked to cite the following paper if any work based upon or utilizing this
    framework is published in the scientific literature: 
    Sherbert, Robert M. and Oh, Paul Y. "Conductor: A Controller Development
    Framework for High Degree of Freedom Systems." Intelligent Robots and
    Systems (IROS), 2011 IEEE/RSJ International Conference on. 
*/

#ifndef ACES_CONTROLLER_HPP
#define ACES_CONTROLLER_HPP

#include <vector>
#include <map>
//#include <list>
#include <iostream>
#include <fstream>
#include <string>

#include <rtt/TaskContext.hpp>
#include <rtt/Activity.hpp>
#include <rtt/InputPort.hpp>
#include <rtt/OutputPort.hpp>
#include <rtt/scripting/Scripting.hpp>

#include "taskcfg.hpp"
//#include "taskmath.hpp"

//TODO - Relocate this definition 
enum WB_CTRL_STATES { WB_CTRL_HALT, WB_CTRL_RUN, WB_CTRL_STEP };
enum scriptState { CTRL_HALT, CTRL_RUN, CTRL_STEP, CTRL_FAIL, CTRL_END };

namespace ACES{
    class Controller :  public ACESTask//, public TaskMath<float>
    {
        public:
            Controller(std::string cfg, std::string args);
            virtual void updateHook(); 
            virtual bool getStateVector(bool echo=0) = 0;
            virtual bool modStateVector();
            bool lastTXcleared();
            void addCtrl(std::string dest, float sp);
            bool deleteCtrl(std::string ctrl);
            bool clearCtrl();
            float checkCtrl(std::string ctrl);
            bool sendCtrl();
            float getSurface(std::string state, std::string attr);

            RTT::OutputPort< std::map<std::string, void*>* > txDownStream;
            RTT::InputPort<int> packetReporter;

        protected:
            std::map<std::string, void*>* curMap;
        private:
            int packetsPerSP;
            int packetCounter;
    };

    class DumbTrajectory : public Controller
    {
        public:
            DumbTrajectory(std::string cfg, std::string args);
            void updateHook();

            void step();
            bool run();
            void halt();
            bool openScript(std::string scriptPath);
            bool getStateVector(bool echo=false);

            std::vector<std::string> states; //! The States we're controlling
            std::string scriptPath;          //! Location of the script file
            std::ifstream scriptFile;        //! Script stream
            scriptState simState;            //! Internal state tracker
    };

    class NullCtrl : public Controller 
    {
        public:
            NullCtrl(std::string config, std::string args);
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

    class UserTrajectory : public Controller{
        public:
            UserTrajectory(std::string config, std::string args);
            void updateHook();
            bool getStateVector(bool echo=false);
            bool getLine();
            bool openTrajectory(std::string sp);
        private:
            std::vector<std::string> states; //! The States we're controlling
            std::ifstream scriptFile;        //! Script stream
            std::string fileName;
            std::string trajName;
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

/*
    class YJCtrl : public LegCtrl {
        public:
            YJCtrl(std::string config, std::string args);
            virtual bool modStateVector();
    };
*/
}

#endif
