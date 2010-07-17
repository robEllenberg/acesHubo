#include "webots.hpp"

namespace Webots {
    Hardware::Hardware(ACES::taskCfg cfg, std::string args)
      : ACES::Hardware(cfg, args)
    {
        RTT::Method<void(int)> *stepMethod = new RTT::Method<void(int)>
            ("step", &Hardware::step, this);
        this->methods()->addMethod(stepMethod,
                                   "Advance the system time",
                                   "TStep", "Lenght of step to advance(ms)");
    }
       
    bool Hardware::startHook(){
        wb_robot_init();
        return true;
    }
    
    void Hardware::updateHook(){
    }

    void Hardware::step(int time){
        ACES::Hardware::updateHook();
        wb_robot_step(time);
    }

    bool Hardware::transmit(ACES::Message* m){
        ACES::Goal *g = m->goalList.front();
        //g->printme();

        switch( g->mode ){
            case (ACES::REFRESH):
                break;
            case (ACES::SET):
                {
                    //g->printme();
                    Credentials* c =
                            (Credentials*)(g->cred);
                    std::string jid = (*c).wb_device_id;

                    //Pull the seek value out of the SValue object
                    float* tp = (float*)(g->data);
                    float target = *tp;
                    float angle = (*c).direction
                                   * (target - (*c).zero);

                    WbDeviceTag joint = wb_robot_get_device(jid.c_str());
                    //wb_servo_set_position(joint, 3.14159/180.*angle);
                    wb_servo_set_position(joint, angle);
                }
                break;
            default:
                break;
        }
        //m->printme();
    }

    bool Hardware::recieve(){
        return true;
    }

    bool Hardware::isBusy(){
        return false;
    }

    bool Hardware::subscribeController(ACES::Controller* c){
        this->connectPeers( (RTT::TaskContext*) c);
        RTT::Handle h = c->events()->setupConnection("applyStateVector")
                .callback( this, &Hardware::stepRequest,
                           this->engine()->events() ).handle();
        if( not h.ready() ){
            return false;
        }
        h.connect();
        if( not h.connected() ){
            return false;
        }
        return true;
    }

    void Hardware::stepRequest( std::map<std::string, void*>* ){
        //TODO - This will advance the simulation before the 
        //new state information is processed
        step();
    }

    Credentials::Credentials(Credentials* c)
        : ACES::Credentials( (ACES::Credentials*)c )
    {
        wb_device_id = c->wb_device_id;
        zero = c->zero;
        direction = c->direction;
    }

    Credentials::Credentials(std::string id_str, float z,
        float dir)
      : ACES::Credentials(ACES::CRED_WB_JOINT){
        wb_device_id = id_str;
        zero = z;
        direction = dir;
    }  

    Credentials::Credentials(char* id_str, float z, 
                float dir)  
      : ACES::Credentials(ACES::CRED_WB_JOINT){
        wb_device_id = (std::string)id_str;
        zero = z;
        direction = dir;
    }

    void Credentials::printme(){
        ACES::Credentials::printme();
        
        RTT::Logger::log() << "Webots: ID= " << wb_device_id;
        RTT::Logger::log() << " Zero= " << zero;
        RTT::Logger::log() << " Direction= " << direction;
        RTT::Logger::log() << RTT::endlog();
    }

    ACES::Credentials* Credentials::parseDispArgs(std::string args)
    {
        std::istringstream s1(args);
        std::string id;
        float zero, rot;
        s1 >> id >> zero >> rot;
        return (ACES::Credentials*)new Credentials(id, zero, rot);
    }

    Device::Device(std::string config, std::string args)
        : ACES::Device(config)
    {
        ACES::Credentials *cred = Webots::Credentials::parseDispArgs(args);
        credentials = cred;
    }

    Protocol::Protocol(std::string cfg, std::string args) 
      : ACES::Protocol(cfg, args){}

    ScriptCtrl::ScriptCtrl(std::string cfg, std::string args)
      : ACES::ScriptCtrl(cfg, args)
    {}

    std::map<std::string, void*>*
      ScriptCtrl::getStateVector(bool echo)
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
}
