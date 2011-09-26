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

#include "controller.hpp"
namespace ACES{

    Controller::Controller(std::string cfg, std::string args)
      : ACESTask(cfg),
        packetsPerSP(0),
        packetCounter(0)
    {
        //Why are all of these operations executed in seperate threads?
        //It doesn't change the execution, since the operation is blocking, and
        //most of these shouldn't need a whole other thread to run.
        //TODO: Try getting rid of these and seeing if it cuts down on
        //processing load.
        this->ports()->addPort("TxDS", txDownStream).doc(
                               "DownStream (to State) Transmission");
        this->addOperation("addCtrl", &Controller::addCtrl, this,
                           RTT::OwnThread)
                           .doc("Add a set point for a controlled quantity")
                           .arg("target", "The controlled quantity")
                           .arg("set point", "The target set point");
        this->addOperation("sendCtrl", &Controller::sendCtrl, this,
                           RTT::OwnThread)
                           .doc("Send the accumulated controls");
        this->addOperation("checkCtrl", &Controller::checkCtrl, this,
                           RTT::OwnThread)
                           .doc("Check a control on the stack")
                           .arg("control", "Control to check");
        this->addOperation("getSurface", &Controller::getSurface, this,
                           RTT::OwnThread).doc("Grab the value of a state")
                           .arg("state", "State to grab value from")
                           .arg("attribute", "Quantity on state to grab");
        this->addOperation("deleteCtrl", &Controller::deleteCtrl, this,
                           RTT::OwnThread)
                           .doc("Clear a part of the set point buffer")
                           .arg("target", "The controlled state to remove");
        this->addOperation("clearCtrl", &Controller::clearCtrl, this,
                           RTT::OwnThread)
                           .doc("Clear the entire set point buffer");
        this->ports()->addPort("packetReport", packetReporter).doc(
                            "Port for Hardware to report reception of setpoints");
        this->addAttribute("packetsPerSP", packetsPerSP);


        curMap = NULL;
    }

    void Controller::addCtrl(std::string dest, float sp){
        if(not curMap){
            curMap = new std::map<std::string, void*>;
        }
        (*curMap)[dest] = (void*) new float(sp);
    }

    bool Controller::sendCtrl(){
        if(not curMap){
            curMap = new std::map<std::string, void*>;
        }
        txDownStream.write(curMap);
        curMap = NULL;
        return true;
    }

    bool Controller::deleteCtrl(std::string ctrl){
        if(curMap){
            map<std::string, void*>::iterator it;
            it = curMap->find(ctrl);
            curMap->erase(it);
            return true;
        }
        return false;
    }

    bool Controller::clearCtrl(){
        curMap = NULL;
        return true;
    }

    float Controller::getSurface(std::string state, std::string attr){
        RTT::TaskContext* t = (TaskContext*)(getPeer(state));
        if(not t){
            return 0;
        }
        //TODO - We need to account for the case where this lookup fails
        RTT::Attribute<float> a =
            t->attributes()->getAttribute(attr);

        return a.get();
    }

    void Controller::updateHook(){
        //if(lastTXcleared()){
            if(getStateVector()){
                modStateVector();
                txDownStream.write(curMap);
                curMap = NULL;
            }
        //}
    }

    bool Controller::lastTXcleared(){
        if(not packetsPerSP){   //if this is >0, we should be paying attention to it
            return true;
        }
        int in = 0;
        while(packetReporter.read(in) == RTT::NewData){
            packetCounter += in;
            in = 0;
        }
        if(packetCounter >= packetsPerSP){
            packetCounter = 0;
            return true;
        }

        return false;
    }

    float Controller::checkCtrl(std::string ctrl){
        std::map<std::string, void*>::iterator it = curMap->find(ctrl);
        if(it != curMap->end()){
            float* f = (float*)(*it).second;
            return *f;
        }
        return 0.0;
    }

    bool Controller::modStateVector(){
        return false;
    }

//---------------Flexscript-----------------------

    DumbTrajectory::DumbTrajectory(std::string cfg, std::string args)
      : Controller(cfg, args),
        scriptPath(args),
        scriptFile()
    {
        //Set the current state of the simulation to stopped
        simState = CTRL_HALT;

        //Make sure we have an existant script file
        openScript(scriptPath);

        this->addOperation("step", &DumbTrajectory::step, this, RTT::OwnThread).doc(
                           "Advance the script by one step.");
        this->addOperation("run", &DumbTrajectory::run, this, RTT::OwnThread).doc(
                           "Start the script (free running).");
        this->addOperation("halt",  &DumbTrajectory::halt, this, RTT::OwnThread).doc(
                           "Halt the script.");
        this->addOperation("openScript", &DumbTrajectory::openScript, this,
                          RTT::OwnThread).doc("Open a new trajectory file").arg(
                          "sfile", "Script file path");           
    }

    void DumbTrajectory::updateHook(){
        switch(simState){
            case CTRL_HALT:
            case CTRL_FAIL:
                break;
            case CTRL_STEP:
                //Fall through here is intentional
            case CTRL_RUN:
                if( getStateVector(true) ){
                    sendCtrl();
                }
                if(simState == CTRL_STEP){
                    simState = CTRL_HALT;
                }
                break;
            default:
                break;
        }
    }

    bool DumbTrajectory::getStateVector(bool echo){
	char buf[1024];
        curMap = new std::map<std::string, void*>;
        scriptFile.getline(buf, 1024);
        if(not scriptFile.eof() ){
            std::string line(buf);
            istringstream lineStream(line);
            for(unsigned int i = 0; i < states.size(); i++){  
                //offset = it+i;
                float value;
                lineStream >> value;
                addCtrl( states[i], value);
                //(*curMap)[ states[i] ] = new float(value);
                //RTT::Logger::log(RTT::Logger::Debug) << value << ", ";
            }
            RTT::Logger::log(RTT::Logger::Debug) << "SP" << RTT::endlog();
            return true;
        }
        else{
            simState = CTRL_FAIL;
        }
        return false;
    }

    void DumbTrajectory::step(){
        if(simState == CTRL_FAIL){
            RTT::Logger::log(RTT::Logger::Warning)
             << "You may not run without a valid script." << RTT::endlog();
        } else{
            simState = CTRL_STEP;
        }
    };

    bool DumbTrajectory::run(){
        if(simState == CTRL_FAIL){
            RTT::Logger::log(RTT::Logger::Warning)
             << "You may not run without a valid script." << RTT::endlog();
        }
        else{
            simState = CTRL_RUN;
        }
        return true;
    }

    void DumbTrajectory::halt(){
        simState = CTRL_HALT;
    }

    bool DumbTrajectory::openScript(std::string sp){
        if(scriptFile.is_open()){
            scriptFile.close();
            //Flush the current state list
            while(states.size()){
                states.pop_back();
            }
        }

        scriptPath = sp;
        scriptFile.open((const char*)scriptPath.c_str(), std::ifstream::in);

        if(scriptFile.is_open()){
            RTT::Logger::log(RTT::Logger::Warning) << "Successfully opened the "
                << "script file at \"" << scriptPath << "\"."
                << RTT::endlog();
            char buf[1024];
            scriptFile.getline(buf, 1024);
            std::string headerLine(buf);
            istringstream s(headerLine);
            while(not s.eof()){
                std::string stateName;
                s >> stateName;
                states.push_back(stateName);
            }
            simState = CTRL_HALT;
            return true;
        }
        else {
            simState = CTRL_FAIL;
            RTT::Logger::log(RTT::Logger::Warning) << "Unable to open the "
                << "script file at \"" << scriptPath << "\"."
                << RTT::endlog();
        }
        return false;
    }
//-----------------/FlexScript--------------

    NullCtrl::NullCtrl(std::string cfg, std::string args)
      : Controller(cfg, args)
    {}

    bool NullCtrl::getStateVector(bool echo){
        curMap = new std::map<std::string, void*>;
        return true; 
    }

    PID::PID(std::string config, std::string args)
      :Controller(config, args)
    {
        std::istringstream s1(args);
        s1 >> inputSurface >> outputSurface  >> Kp >> Ki >> Kd;
        addAttribute("Kp", Kp);
        addAttribute("Ki", Ki);
        addAttribute("Kd", Kd);
        addAttribute("Kd", val);
        addAttribute("Input", inputSurface);
        addAttribute("Output", outputSurface);
    }

    bool PID::getStateVector(bool echo){
        curMap = new std::map<std::string, void*>;
        float p = getSurface(inputSurface, "value");
        float i = getSurface(inputSurface, "integral");
        float d = getSurface(inputSurface, "diff");
        val = Kp*p + Ki*i + Kd*d;

        (*curMap)[outputSurface] = (void*) new float(val);
        return true;
    }

    UserTrajectory::UserTrajectory(std::string config, std::string args)
     :Controller(config, args)
    {
        std::istringstream s1(args);
        s1 >> fileName >> trajName;

        this->addOperation("getLine", &UserTrajectory::getLine,
                            this, RTT::OwnThread)
                           .doc("Open a new trajectory file");
        this->addOperation("openTrajectory", &UserTrajectory::openTrajectory,
                            this, RTT::OwnThread)
                           .doc("Open a new trajectory file");

        openTrajectory(trajName);
        getProvider<RTT::Scripting>("scripting")->loadPrograms(fileName);
    }

    void UserTrajectory::updateHook(){
    }

    bool UserTrajectory::openTrajectory(std::string sp){
        if(scriptFile.is_open()){
            scriptFile.close();
            //Flush the current state list
            while(states.size()){
                states.pop_back();
            }
        }

        scriptFile.open((const char*)trajName.c_str(), std::ifstream::in);

        if(scriptFile.is_open()){
            RTT::Logger::log(RTT::Logger::Warning) << "Successfully opened the "
                << "script file at \"" << trajName << "\"."
                << RTT::endlog();
            char buf[1024];
            scriptFile.getline(buf, 1024);
            std::string headerLine(buf);
            istringstream s(headerLine);
            while(not s.eof()){
                std::string stateName;
                s >> stateName;
                states.push_back(stateName);
            }
            return true;
        }
        else {
            RTT::Logger::log(RTT::Logger::Warning) << "Unable to open the "
                << "script file at \"" << trajName << "\"."
                << RTT::endlog();
        }
        return false;
    }

    bool UserTrajectory::getStateVector(bool echo){
        return false;
    }

    bool UserTrajectory::getLine(){
        char buf[1024];
        scriptFile.getline(buf, 1024);
        if(not scriptFile.eof() ){
            std::string line(buf);
            istringstream lineStream(line);
            for(unsigned int i = 0; i < states.size(); i++){  
                //offset = it+i;
                float value;
                lineStream >> value;
                addCtrl( states[i], value);
                //RTT::Logger::log(RTT::Logger::Debug) << value << ", ";
            }
            //RTT::Logger::log(RTT::Logger::Debug) << RTT::endlog();
            return true;
        }
        return false;
    }

    UserProg::UserProg(std::string config, std::string args)
     :Controller(config, args)
    {
        std::istringstream s1(args);
        s1 >> filename >> progname;
        getProvider<RTT::Scripting>("scripting")->loadPrograms(filename);
    }

    bool UserProg::startHook(){
        //getProvider<RTT::Scripting>("scripting")->startProgram(progname);
        return true;
    }

    bool UserProg::getStateVector(bool echo){
        return false;
    }

    UserSM::UserSM(std::string config, std::string args)
     :Controller(config, args)
    {
        std::istringstream s1(args);
        s1 >> filename >> progname;
        getProvider<RTT::Scripting>("scripting")->loadStateMachines(filename);
    }

    bool UserSM::getStateVector(bool echo){
        return false;
    }

    bool UserSM::startHook(){
        getProvider<RTT::Scripting>("scripting")->
                                   activateStateMachine(progname);
        //getProvider<RTT::Scripting>("scripting")->
        //                           startStateMachine(progname);
        return true;
    }

/*
    YJCtrl::YJCtrl(std::string config, std::string args)
     : LegCtrl(config, args){}

    bool YJCtrl::modStateVector(){
        float rap = checkCtrl("RAP");
        float RftX = getSurface("rffX", "value");
        float RftY = getSurface("rffY", "value");
        float RftZ = getSurface("rffZ", "value");
        float temp = rap*1.0/3.0*(RftX+RftY+RftZ);
        addCtrl("RSP", temp);
        return true;
    }
*/
}
