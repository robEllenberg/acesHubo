#include "controller.hpp"
namespace ACES{

    WbController::WbController( std::string n,
      const char* scriptFile, 
      int pri, int UpdateFreq)
      : RTT::TaskContext(n),
        walkScript(scriptFile, std::ifstream::in),
        applyStateVector("applyStateVector"),
        stepMethod("step", &WbController::step, this),
        runMethod("run", &WbController::run, this),
        haltMethod("halt", &WbController::halt, this),
        openScriptMethod("openScript", &WbController::openScript, this)
    {
        //Copy some important parameters onto the object
        name = n;                 //Name
        frequency = UpdateFreq;
        priority = pri;

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
/*
        std::vector<ACES::State*>::iterator it;
        for(it = plist.begin(); it != plist.end(); it++){
           //Create a mapping of names->states
           pmap[(*it)->name] = (*it);
           //Connect and verifyeach of the states to the state
           //application event
           RTT::Handle h = this->events()->setupConnection("applyStateVector")
                .callback( (*it), &State::setGoal,
                           (*it)->engine()->events() ).handle();
           assert( h.ready() );
           h.connect();
        }
*/
        //Create the governing activity
        this->setActivity(
            new RTT::Activity( priority, 1.0/UpdateFreq, 0, n )
        );
    }
    
    WbController::WbController(taskCfg cfg, std::string args)
      : RTT::TaskContext(cfg.name),
        walkScript((const char*)args.c_str(), std::ifstream::in),
        applyStateVector("applyStateVector"),
        stepMethod("step", &WbController::step, this),
        runMethod("run", &WbController::run, this),
        haltMethod("halt", &WbController::halt, this),
        openScriptMethod("openScript", &WbController::openScript, this)
    {
        name = cfg.name;
        priority = cfg.priority;
        frequency = cfg.freq;

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
            new RTT::Activity( priority, 1.0/frequency, 0, cfg.name )
        );
    }

    void WbController::updateHook(){
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

    bool WbController::configureHook(){
        return true;
    }

    bool WbController::startHook(){
        return true;
    }

    void WbController::stopHook(){
    }

    void WbController::cleanupHook(){}

    void WbController::step(){
        simState = WB_CTRL_STEP;
    }

    void WbController::run(){
        simState = WB_CTRL_RUN;
    }

    void WbController::halt(){
        simState = WB_CTRL_HALT;
    }

    bool WbController::openScript(std::string scriptPath){
        if(walkScript.is_open()){
            walkScript.close();
        }
        walkScript.open((const char*)scriptPath.c_str());

        if(walkScript.is_open())
            return true;
        else
            return false;
    }

    std::map<std::string, void*>*
      WbController::getStateVector(bool echo){
        //The state vector is a lookup table by the name of the joint
        std::map<std::string, void*> *sv =
            new std::map<std::string, void*>;

        std::vector<float> angles;      //Temp container
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
        }
        if(echo){
            RTT::Logger::log() << RTT::endlog();
        }

        //Eat the remainder of the line
        char a[1000];
        walkScript.getline(a, 1000);

        //Populate the state vector
        (*sv)["HY"] = new float(angles[0]);
        (*sv)["LHY"] = new float(angles[1]);
        (*sv)["LHR"] = new float(angles[2]);
        (*sv)["LHP"] = new float(angles[3]);
        (*sv)["LKP"] = new float(angles[4]);
        (*sv)["LAP"] = new float(angles[5]);
        (*sv)["LAR"] = new float(angles[6]);
        (*sv)["RHY"] = new float(angles[7]);
        (*sv)["RHR"] = new float(angles[8]);
        (*sv)["RHP"] = new float(angles[9]);
        (*sv)["RKP"] = new float(angles[10]);
        (*sv)["RAP"] = new float(angles[11]);
        (*sv)["RAR"] = new float(angles[12]);
 
        return sv;
    }
}
