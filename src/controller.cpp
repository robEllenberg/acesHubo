#include "controller.hpp"
namespace ACES{

    Controller::Controller(std::string cfg, std::string args)
      : taskCfg(cfg),
        RTT::TaskContext(name)
    {
        //this->events()->addEvent(&applyStateVector, "applyStateVector",
        //    "SVmap", "map of state vect info");

        this->ports()->addPort("TxDS", txDownStream).doc(
                               "DownStream (to State) Transmission");

        this->setActivity(
            new RTT::Activity( priority, 1.0/freq, 0, name )
        );
    }

    ScriptCtrl::ScriptCtrl(std::string cfg, std::string args)
      : Controller(cfg, args),
        walkScript((const char*)args.c_str(), std::ifstream::in)
        //stepMethod("step", &ScriptCtrl::step, this),
        //runMethod("run", &ScriptCtrl::run, this),
        //haltMethod("halt", &ScriptCtrl::halt, this),
        //openScriptMethod("openScript", &ScriptCtrl::openScript, this)
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

        //this->methods()->addMethod(stepMethod,
        //    "Advance the simulation one time step.");
        //this->methods()->addMethod(runMethod,
        //    "Start the simulation (free running).");
        //this->methods()->addMethod(haltMethod,
        //    "Halt the simulation.");
        //this->methods()->addMethod(openScriptMethod,
        //    "Open a new trajectory file", "sfile", "Script file path");
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
                txDownStream.write(sv);
                
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

    void NullCtrl::updateHook()
    {
        std::map<std::string, void*>* sv;
        sv = getStateVector();
        txDownStream.write(sv);
    }

    std::map<std::string, void*>*
      NullCtrl::getStateVector(bool echo)
    {
        std::map<std::string, void*> *sv =
            new std::map<std::string, void*>;
        return sv; 
    }

    LegCtrl::LegCtrl(std::string cfg, std::string args)
      : ScriptCtrl(cfg, args)
    {}

    std::map<std::string, void*>*
      LegCtrl::getStateVector(bool echo)
    {
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
            //Issue an empty vector if we don't want to do anything
            //(picked up by HW to advance timestep in sim)
            return sv;
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
        //(*sv)["RSP"] = new float(1.2);
 
        return sv;
    }

    ArmCtrl::ArmCtrl(std::string cfg, std::string args)
      : ScriptCtrl(cfg, args)
    {}

    std::map<std::string, void*>*
      ArmCtrl::getStateVector(bool echo)
    {
        //The state vector is a lookup table by the name of the joint
        std::map<std::string, void*> *sv =
            new std::map<std::string, void*>;

        std::vector<float> angles;      //Temp container
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
           return sv;
        }
        if(echo){
            RTT::Logger::log() << RTT::endlog();
        }

        //Eat the remainder of the line
        char a[1000];
        walkScript.getline(a, 1000);

        //Populate the state vector
        (*sv)["RSP"] = new float(angles[0]);
        (*sv)["LSP"] = new float(angles[1]);

        return sv;
    }

}
