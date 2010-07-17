#include "controller.hpp"
namespace ACES{

    Controller::Controller(std::string cfg, std::string args)
      : taskCfg(cfg),
        RTT::TaskContext(name)
    {
    }

    bool Controller::configureHook(){
        return true;
    }

    bool Controller::startHook(){
        return true;
    }

    void Controller::stopHook(){
    }

    void Controller::cleanupHook(){}

    ScriptCtrl::ScriptCtrl(std::string cfg, std::string args)
      : Controller(cfg, args),
        walkScript((const char*)args.c_str(), std::ifstream::in),
        applyStateVector("applyStateVector"),
        stepMethod("step", &ScriptCtrl::step, this),
        runMethod("run", &ScriptCtrl::run, this),
        haltMethod("halt", &ScriptCtrl::halt, this),
        openScriptMethod("openScript", &ScriptCtrl::openScript, this)
    {
        //Set the current state of the simulation to stopped
        simState = WB_CTRL_HALT;

        //Make sure we have an existant script file
        assert(walkScript.is_open());

        this->methods()->addMethod(stepMethod,
            "Advance the simulation one time step.");
        this->methods()->addMethod(runMethod,
            "Start the simulation (free running).");
        this->methods()->addMethod(haltMethod,
            "Halt the simulation.");
        this->methods()->addMethod(openScriptMethod,
            "Open a new trajectory file", "sfile", "Script file path");

        this->events()->addEvent(&applyStateVector, "applyStateVector",
            "SVmap", "map of state vect info");

        this->setActivity(
            new RTT::Activity( priority, 1.0/freq, 0, name )
        );
    }
    
    void ScriptCtrl::updateHook(){
        switch(simState){
            case WB_CTRL_HALT:
                break;
            case WB_CTRL_STEP:
                //Fall through here is intentional
            case WB_CTRL_RUN:
                 //delete stateVect;
                stateVect = getStateVector(true);
                applyStateVector(stateVect);
                
                //delete sv;       
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

    void ScriptCtrl::run(){
        simState = WB_CTRL_RUN;
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
}
