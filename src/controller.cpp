#include "controller.hpp"
namespace ACES{

    WbController::WbController(
      std::string n, std::vector<ACES::Parameter*> pl,
      const char* scriptFile, ACES::Hardware* hw_one,
      ACES::Dispatcher* dispatch,
      int pri, int UpdateFreq)
      : RTT::TaskContext(n),
        walkScript(scriptFile, std::ifstream::in),
        applyStateVector("applyStateVector")
        //stepMethod("Step", &WbController::step, this)
    {
        RTT::Method<void(void)>* stepMethod = new RTT::Method<void(void)>
            ("step", &WbController::step, this);
        this->methods()->addMethod(stepMethod, "Advance the simulation one time step.");

        this->events()->addEvent(&applyStateVector, "applyStateVector", "SVmap", "map of state vect info");

        assert(walkScript.is_open());
        plist = pl;
        std::vector<ACES::Parameter*>::iterator it;

        //Create a mapping of names->parameters
        for(it = plist.begin(); it != plist.end(); it++){
           pmap[(*it)->name] = (*it);
           RTT::Handle h = this->events()->setupConnection("applyStateVector")
                .callback( (*it), &Parameter::setGoal,
                           (*it)->engine()->events() ).handle();
           assert( h.ready() );
           h.connect();
        }
        name = n;
        hwlist.push_back(hw_one);
        dispatcher = dispatch;
        frequency = UpdateFreq;
        priority = pri;
        this->setActivity(
            new RTT::Activity( priority, 1.0/UpdateFreq )
        );
    }

    void WbController::updateHook(){
        step();
    }

    bool WbController::configureHook(){
        return true;
    }

    bool WbController::startHook(){
        for(std::list<ACES::Hardware*>::iterator it = hwlist.begin();
            it != hwlist.end(); it++){
                (*it)->start();
        }
        dispatcher->start();
        return true;
    }

    void WbController::stopHook(){
        dispatcher->stop();
        for(std::list<ACES::Hardware*>::iterator it = hwlist.begin();
            it != hwlist.end(); it++){
                (*it)->stop();
        }
    }

    void WbController::cleanupHook(){}

    void WbController::step(){
        //delete stateVect;
        for(std::list<ACES::Hardware*>::iterator it = hwlist.begin();
            it != hwlist.end(); it++){
                RTT::Method<void(int)> stepper = 
                        (*it)->methods()->getMethod<void(int)>("step");
                //for(int i = 0; i< 10000; i++){
                //    int a = 2;
                //}
                stepper(32);
        }
        stateVect = getStateVector();
        applyStateVector(stateVect);
        //delete sv;
    }

    std::map<std::string, ACES::PValue*>* WbController::getStateVector(){

        std::map<std::string, ACES::PValue*> *sv =
            new std::map<std::string, ACES::PValue*>;

        std::vector<float> angles;
        //Fill w/the script info if we have data left, 
        //otherwise zero fill the vector
        if(not walkScript.eof()){
            for(int i = 0; i<13; i++){
                //offset = it+i;
                float value;
                walkScript >> value;
                angles.push_back(value);
               RTT::Logger::log() << value << ", ";
            }
        }else{
            for(int i=0; i< 13; i++){
                angles.push_back(0.0);
                RTT::Logger::log() << "EOF" << ", ";
            }
        }
        RTT::Logger::log() << RTT::endlog();

        //Eat the remainder of the line
        char a[1000];
        walkScript.getline(a, 1000);

        //Populate the state vector
        /*
        (*sv)["HY"] = new ACES::PValue(angles[0]);
        (*sv)["LHY"] = new ACES::PValue(angles[1]);
        (*sv)["LHR"] = new ACES::PValue(angles[2]);
        (*sv)["LHP"] = new ACES::PValue(angles[3]);
        (*sv)["LKP"] = new ACES::PValue(angles[4]
                                 + angles[5]);
        (*sv)["LAP"] = new ACES::PValue(angles[6]);
        (*sv)["LAR"] = new ACES::PValue(angles[7]);
        (*sv)["RHY"] = new ACES::PValue(angles[8]);
        (*sv)["RHR"] = new ACES::PValue(angles[9]);
        (*sv)["RHP"] = new ACES::PValue(angles[10]);
        (*sv)["RKP"] = new ACES::PValue(angles[11]
                                 + angles[12]);
        (*sv)["RAP"] = new ACES::PValue(angles[13]);
        (*sv)["RAR"] = new ACES::PValue(angles[14]);
        */
        (*sv)["HY"] = new ACES::PValue(angles[0]);
        (*sv)["LHY"] = new ACES::PValue(angles[1]);
        (*sv)["LHR"] = new ACES::PValue(angles[2]);
        (*sv)["LHP"] = new ACES::PValue(angles[3]);
        (*sv)["LKP"] = new ACES::PValue(angles[4]);
        (*sv)["LAP"] = new ACES::PValue(angles[5]);
        (*sv)["LAR"] = new ACES::PValue(angles[6]);
        (*sv)["RHY"] = new ACES::PValue(angles[7]);
        (*sv)["RHR"] = new ACES::PValue(angles[8]);
        (*sv)["RHP"] = new ACES::PValue(angles[9]);
        (*sv)["RKP"] = new ACES::PValue(angles[10]);
        (*sv)["RAP"] = new ACES::PValue(angles[11]);
        (*sv)["RAR"] = new ACES::PValue(angles[12]);
 
        return sv;
    }

/*
    void WbController::applyStateVector
      (std::map<std::string, ACES::PValue*>& sv){
        //p = parameter, 
        std::map<std::string, ACES::Parameter* >::iterator p;
        std::map<std::string, ACES::PValue*>::iterator state;
        for(state = sv.begin(); state != sv.end(); state++){
            p = pmap.find( (*state).first );
            if(p != pmap.end()){
                ACES::Parameter* param = (*p).second;
                ACES::PValue* goal = (*state).second;
                RTT::Command<bool(ACES::PValue*)> *pUpdate =
                    param->setPoint;
                //while( not pUpdate->ready() ){}
                (*pUpdate)(goal);
            }
        }
    }
*/

}
