#include "webots.hpp"

namespace Webots {
    
    Hardware::Hardware(std::string name, int pri,
                     int UpdateFreq) 
      :ACES::Hardware(name, pri, UpdateFreq){
        
        RTT::Method<void(int)> *stepMethod = new RTT::Method<void(int)>
            ("step", &Hardware::step, this);
        this->methods()->addMethod(stepMethod);
            //"Time to advance (ms)");
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
        //Pull the first element out of the abstrated
        //credentials list and cast it to a webots
        //credentials.
        
        Credentials* c = (Credentials*)(m->credList.front());
        std::string jid = (*c).wb_device_id;

        //Pull the seek value out of the PValue object
        if( (*c).val ){
            float target = (*c).val->val;
            float angle = (*c).rotation
                           * (target - (*c).zero);

            WbDeviceTag joint = wb_robot_get_device(jid.c_str());
            //wb_servo_set_position(joint, 3.14159/180.*angle);
            wb_servo_set_position(joint, angle);
        }
        //m->printme();
    }

    bool Hardware::recieve(){
        return true;
    }

    bool Hardware::isBusy(){
        return false;
    }

    Credentials::Credentials() : ACES::Credentials(){
        wb_device_id = "";
        zero = 0.0;
        rotation = 1.0;
        val = 0;
    }
    
    Credentials::Credentials(std::string id_str, float z,
        float rot)
      : ACES::Credentials(){
        wb_device_id = id_str;
        zero = z;
        rotation = rot;
    }  

    Credentials::Credentials(char* id_str, float z, 
                float rot){
        Credentials( (std::string)id_str, z, rot );
    }

    Credentials::Credentials(Credentials& c, ACES::PValue* p)
      : ACES::Credentials(0, p){ 
        wb_device_id = c.wb_device_id;
        zero = c.zero;
        rotation = c.rotation;
    }

    void Credentials::printme(){
        int* p = (int*)val;
        RTT::Logger::log() << "Credential w/id= " << this->wb_device_id
            << " and value= ";
        if(p){
            this->val->printme();
        }
        else{
            RTT::Logger::log()  << "NULL";
        }
        RTT::Logger::log() << RTT::endlog();
    }

    ACES::Credentials* Credentials::credCopy(ACES::PValue* p){
        Credentials* c = new Credentials();
        c->wb_device_id = wb_device_id;
        c->zero = zero;
        c->rotation = rotation;
        c->val = p;
        return (ACES::Credentials*) c;
    }

    Protocol::Protocol(std::string name, Hardware* hw,
                     int pri, int UpdateFreq)
      : ACES::Protocol(name, hw, pri, UpdateFreq){}

    void Protocol::aggregateRequests(
      std::list<ACES::Credentials*> &reqs){
        while( not reqs.empty() ){
            ACES::Message *m = new ACES::Message( reqs.front() );
            reqs.pop_front();
            pending_stack->push_back(m);
        }
    }

    ACES::Credentials* Protocol::parseHWInput(
                       ACES::Message* c) {}

    State::State(std::string n,
      ACES::Credentials* c,
      int pri, int UpdateFreq)
      : ACES::State(n, c, pri, UpdateFreq)
    {}

    State::State(std::string pname, std::string cname,
      int pri, int UpdateFreq,
      float z, float rot)
        : ACES::State(pname,
                          new Credentials(cname,z,rot),
                          pri, UpdateFreq)
    {}
}
