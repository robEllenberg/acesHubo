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
                    g->printme();
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

    bool Hardware::subscribeController(ACES::WbController* c){
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

    Credentials::Credentials(Credentials* c, void* sp)
        : ACES::Credentials( (ACES::Credentials*)c )
    {
        wb_device_id = c->wb_device_id;
        zero = c->zero;
        direction = c->direction;
        //setPoint = (void*) new float(sp);
        //setPoint = sp;
    }
/*
    ACES::Credentials* Credentials::copy(void* setP){
        Credentials* c = new Credentials(this);
        NCcopy((ProtoCredential*)c);
        c->setPoint = setP;
        return (ACES::Credentials*)c;
    }
*/
    void Credentials::printme(){
        //int* p = (int*)val;
        ACES::Credentials::printme();
        
        RTT::Logger::log() << "Webots ID= " << this->wb_device_id;
        //    << " and SetPoint= ";
        //if(p){
        /*
        if(this->setPoint){
            //this->val->printme();
            float* v = (float*)setPoint;
            RTT::Logger::log() << *v;
        }
        else{
            RTT::Logger::log()  << "NULL";
        }
        */
        RTT::Logger::log() << RTT::endlog();
    }

    ACES::Credentials* Credentials::parseDispArgs(std::string args)
    {
        ACES::Credentials *c = 0;
        std::istringstream s1(args);
        std::string id;
        float zero, rot;
        s1 >> id >> zero >> rot;
        c = (ACES::Credentials*)new Credentials(id, zero, rot);
        return c;
    }

    Device::Device(std::string config, std::string args)
        : ACES::Device(config)
    {
        ACES::Credentials *cred = Webots::Credentials::parseDispArgs(args);
        credentials = cred;
    }

    Protocol::Protocol(std::string name, 
                     int pri, int UpdateFreq)
      : ACES::Protocol(name, pri, UpdateFreq){}

    Protocol::Protocol(ACES::taskCfg cfg, std::string args) 
      : ACES::Protocol(cfg, args){}

}
