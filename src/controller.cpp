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

#include "controller.hpp"
namespace ACES{

    Controller::Controller(std::string cfg, std::string args)
      : taskCfg(cfg),
        RTT::TaskContext(name)
    {
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

        this->setActivity(
            new RTT::Activity( priority, 1.0/freq, 0, name ));
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
        if(getStateVector()){
            modStateVector();
            txDownStream.write(curMap);
            curMap = NULL;
        }
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

    ScriptCtrl::ScriptCtrl(std::string cfg, std::string args)
      : Controller(cfg, args),
        walkScript((const char*)args.c_str(), std::ifstream::in)
    {
        //Set the current state of the simulation to stopped
        simState = WB_CTRL_HALT;

        //Make sure we have an existant script file
        assert(walkScript.is_open());

        this->addOperation("step", &ScriptCtrl::step, this, RTT::OwnThread).doc(
                           "Advance the simulation one time step.");
        this->addOperation("run", &ScriptCtrl::run, this, RTT::OwnThread).doc(
                           "Start the simulation (free running).");
        this->addOperation("halt",  &ScriptCtrl::halt, this, RTT::OwnThread).doc(
                           "Halt the simulation.");
        this->addOperation("openScript", &ScriptCtrl::openScript, this,
                          RTT::OwnThread).doc("Open a new trajectory file").arg(
                          "sfile", "Script file path");           
    }

    void ScriptCtrl::updateHook(){
        switch(simState){
            case WB_CTRL_HALT:
                break;
            case WB_CTRL_STEP:
                //Fall through here is intentional
            case WB_CTRL_RUN:
                if( getStateVector() ){
                    modStateVector();
                    txDownStream.write(curMap);
                    curMap = NULL;
                }
                if(simState == WB_CTRL_STEP){
                    simState = WB_CTRL_HALT;
                }
                break;
            default:
                break;
        }
    }
    void ScriptCtrl::step(){
        simState = WB_CTRL_STEP;
    }

    bool ScriptCtrl::run(){
        simState = WB_CTRL_RUN;
        return true;
    }

    void ScriptCtrl::halt(){
        simState = WB_CTRL_HALT;
    }

    bool ScriptCtrl::openScript(std::string scriptPath){
        if(walkScript.is_open()){
            walkScript.close();
        }
        walkScript.open((const char*)scriptPath.c_str());

        if(walkScript.is_open())
            return true;
        else
            return false;
    }

    NullCtrl::NullCtrl(std::string cfg, std::string args)
      : Controller(cfg, args)
    {}

    bool NullCtrl::getStateVector(bool echo){
        curMap = new std::map<std::string, void*>;
        return true; 
    }

    LegCtrl::LegCtrl(std::string cfg, std::string args)
      : ScriptCtrl(cfg, args)
    {}

    bool LegCtrl::getStateVector(bool echo)
    {
        //The state vector is a lookup table by the name of the joint

        std::vector<float> angles;      //Temp container
        curMap = new std::map<std::string, void*>;
        //Fill w/the script info if we have data left, 
        //otherwise zero fill the vector
        if(not walkScript.eof()){
            //For the moment, 13 is magic, based on the #of joints and
            //the length of the script-file format.
            for(int i = 0; i<13; i++){  
                //offset = it+i;
                float value;
                walkScript >> value;
                angles.push_back(value);
               if(echo){
                    RTT::Logger::log() << value << ", ";
               }
            }
        }else{
            for(int i=0; i< 13; i++){
                angles.push_back(0.0);
                if(echo){
                    RTT::Logger::log() << "EOF" << ", ";
                }
            }
            //Issue an empty vector if we don't want to do anything
            //(picked up by HW to advance timestep in sim)
            return true;
        }
        if(echo){
            RTT::Logger::log() << RTT::endlog();
        }

        //Eat the remainder of the line
        char a[1000];
        walkScript.getline(a, 1000);

        //Populate the state vector
        (*curMap)["HY"] = new float(angles[0]);
        (*curMap)["LHY"] = new float(angles[1]);
        (*curMap)["LHR"] = new float(angles[2]);
        (*curMap)["LHP"] = new float(angles[3]);
        (*curMap)["LKP"] = new float(angles[4]);
        (*curMap)["LAP"] = new float(angles[5]);
        (*curMap)["LAR"] = new float(angles[6]);
        (*curMap)["RHY"] = new float(angles[7]);
        (*curMap)["RHR"] = new float(angles[8]);
        (*curMap)["RHP"] = new float(angles[9]);
        (*curMap)["RKP"] = new float(angles[10]);
        (*curMap)["RAP"] = new float(angles[11]);
        (*curMap)["RAR"] = new float(angles[12]);
        //(*curMap)["RSP"] = new float(1.2);
 
        return true;
    }

    ArmCtrl::ArmCtrl(std::string cfg, std::string args)
      : ScriptCtrl(cfg, args)
    {}

    bool ArmCtrl::getStateVector(bool echo)
    {
        //The state vector is a lookup table by the name of the joint
        std::vector<float> angles;      //Temp container
        curMap = new std::map<std::string, void*>;
        //Fill w/the script info if we have data left, 
        //otherwise zero fill the vector
        if(not walkScript.eof()){
            //For the moment, 13 is magic, based on the #of joints and
            //the length of the script-file format.
            for(int i = 0; i<2; i++){  
                //offset = it+i;
                float value;
                walkScript >> value;
                angles.push_back(value);
               if(echo){
                    RTT::Logger::log() << value << ", ";
               }
            }
        }else{
           return true;
        }
        if(echo){
            RTT::Logger::log() << RTT::endlog();
        }

        //Eat the remainder of the line
        char a[1000];
        walkScript.getline(a, 1000);

        //Populate the state vector
        (*curMap)["RSP"] = new float(angles[0]);
        (*curMap)["LSP"] = new float(angles[1]);

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
}
