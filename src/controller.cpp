#include "controller.hpp"
namespace ACES{

    Controller::Controller(std::string cfg, std::string args)
      : taskCfg(cfg),
        RTT::TaskContext(name),
        applyStateVector("applyStateVector")
    {
        this->events()->addEvent(&applyStateVector, "applyStateVector",
            "SVmap", "map of state vect info");

        this->setActivity(
            new RTT::Activity( priority, 1.0/freq, 0, name )
        );
    }

    Controller::Controller(std::string name)
      : RTT::TaskContext(name),
        applyStateVector("applyStateVector")
    {
        this->events()->addEvent(&applyStateVector, "applyStateVector",
            "SVmap", "map of state vect info");
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
    }

    ScriptCtrl::ScriptCtrl(std::string name)
      : Controller(name),
        //walkScript((const char*)args.c_str(), std::ifstream::in),
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
    }
    
    void ScriptCtrl::updateHook(){
        switch(simState){
            case WB_CTRL_HALT:
                break;
            case WB_CTRL_STEP:
                //Fall through here is intentional
            case WB_CTRL_RUN:
                 //delete stateVect;
                std::map<std::string, void*>* sv;
                sv = getStateVector();
                applyStateVector(sv);
                
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

    NullCtrl::NullCtrl(std::string name)
      : Controller(name)
    {}

    void NullCtrl::updateHook()
    {
        std::map<std::string, void*>* sv;
        sv = getStateVector();
        applyStateVector(sv);
    }

    std::map<std::string, void*>*
      NullCtrl::getStateVector(bool echo)
    {
        std::map<std::string, void*> *sv =
            new std::map<std::string, void*>;
        return sv; 
    }
}
