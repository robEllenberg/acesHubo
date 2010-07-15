#include "webots.hpp"

namespace Webots {
    
    Hardware::Hardware(std::string name, int pri,
                     int UpdateFreq) 
      :ACES::Hardware(name, pri, UpdateFreq){
        
        RTT::Method<void(int)> *stepMethod = new RTT::Method<void(int)>
            ("step", &Hardware::step, this);
        this->methods()->addMethod(stepMethod,
                                   "Advance the system time",
                                   "TStep", "Lenght of step to advance(ms)");
            //"Time to advance (ms)");
    }

    Hardware::Hardware(ACES::taskCfg cfg, std::string args)
      : ACES::Hardware(cfg, args){

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
        //Pull the first element out of the abstrated
        //credentials list and cast it to a webots
        //credentials.
    
        //((ACES::Credentials*)m->credList.front())->printme();

        Credentials *p =
            (Credentials*)(m->credList.front());

        switch( p->credType ){
            case (ACES::CRED_WB_JOINT):
                {
                    Credentials* c =
                            (Credentials*)(m->credList.front());
                    std::string jid = (*c).wb_device_id;

                    //Pull the seek value out of the SValue object
                    if( c->setPoint ){
                        float* tp = (float*)(c->setPoint);
                        float target = *tp;
                        float angle = (*c).direction
                                       * (target - (*c).zero);

                        WbDeviceTag joint = wb_robot_get_device(jid.c_str());
                        //wb_servo_set_position(joint, 3.14159/180.*angle);
                        wb_servo_set_position(joint, angle);
                    }
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


/*
    ACES::Credentials* Credentials::credCopy(void* p){
        Credentials* c = new Credentials();
        c->wb_device_id = wb_device_id;
        c->zero = zero;
        c->rotation = rotation;
        c->val = p;
        return (ACES::Credentials*) c;
    }
*/
 
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
        setPoint = sp;
    }

    ACES::Credentials* Credentials::copy(void* setP){
        Credentials* c = new Credentials(this);
        NCcopy((ProtoCredential*)c);
        c->setPoint = setP;
        return (ACES::Credentials*)c;
    }

    void Credentials::printme(){
        //int* p = (int*)val;
        ACES::Credentials::printme();
        
        RTT::Logger::log() << "Webots ID= " << this->wb_device_id
            << " and SetPoint= ";
        //if(p){
        if(this->setPoint){
            //this->val->printme();
            float* v = (float*)setPoint;
            RTT::Logger::log() << *v;
        }
        else{
            RTT::Logger::log()  << "NULL";
        }
        RTT::Logger::log() << RTT::endlog();
    }

    ACES::Credentials* Credentials::parseDispArgs(std::string type,
                                         std::string args)
    {
        ACES::Credentials *c = 0;
        if(type == "float"){
            std::istringstream s1(args);
            std::string id;
            float zero, rot;
            s1 >> id >> zero >> rot;
            c = (ACES::Credentials*)new Credentials(id, zero, rot);
        }
        return c;
    }

    Protocol::Protocol(std::string name, 
                     int pri, int UpdateFreq)
      : ACES::Protocol(name, pri, UpdateFreq){}

    Protocol::Protocol(ACES::taskCfg cfg, std::string args) 
      : ACES::Protocol(cfg, args){}
        
/*

    void Protocol::aggregateRequests(
      std::list<ACES::Credentials*> &reqs){
        while( not reqs.empty() ){
            ACES::Message *m = new ACES::Message( reqs.front() );
            reqs.pop_front();
            pending_stack->push_back(m);
        }
    }
*/
/*
    ACES::Credentials* Protocol::parseHWInput(
                       ACES::Message* c) {}
*/

/*    
    template <class T>
    void* State<T>::parseDispArgs(std::string type,
                                         std::string args)
    {
        void *c = 0;
        if(type == "float"){
            std::istringstream s1(args);
            std::string id;
            float zero, rot;
            s1 >> id >> zero >> rot;
            c = (void*)new Credentials<float>(id, zero, rot);
        }
        return c;
    }
*/
}
