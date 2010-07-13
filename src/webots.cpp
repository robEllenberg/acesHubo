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

    Hardware::Hardware(ACES::taskCfg cfg, std::string args)
      : ACES::Hardware(cfg, args){

        RTT::Method<void(int)> *stepMethod = new RTT::Method<void(int)>
            ("step", &Hardware::step, this);
        this->methods()->addMethod(stepMethod);
        
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
        ACES::ProtoCredential *p =
            (ACES::ProtoCredential*)(m->credList.front());

        switch( p->credType ){
            case CRED_WB_JOINT:
                {
                Credentials<float>* c =
                        (Credentials<float>*)(m->credList.front());
                std::string jid = (*c).wb_device_id;

                //Pull the seek value out of the SValue object
                if( (*c).val ){
                    float target = (*c).val;
                    float angle = (*c).rotation
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

    template <class T>
    Credentials<T>::Credentials() : ACES::Credentials<T>(){
        wb_device_id = "";
        zero = 0.0;
        rotation = 1.0;
    }
    
    template <class T>
    Credentials<T>::Credentials(std::string id_str, float z,
        float rot)
      : ACES::Credentials<T>(){
        wb_device_id = id_str;
        zero = z;
        rotation = rot;
    }  

    template <class T>
    Credentials<T>::Credentials(char* id_str, float z, 
                float rot){
        Credentials( (std::string)id_str, z, rot );
    }

    template <class T>
    Credentials<T>::Credentials(Credentials<T>& c, ACES::SValue* p)
      : ACES::Credentials<T>(0, p){ 
        wb_device_id = c.wb_device_id;
        zero = c.zero;
        rotation = c.rotation;
    }

    template <class T>
    void Credentials<T>::printme(){
        //int* p = (int*)val;
        RTT::Logger::log() << "Credential w/id= " << this->wb_device_id
            << " and value= ";
        //if(p){
        if(this->val){
            //this->val->printme();
            RTT::Logger::log() << this->val;
        }
        else{
            RTT::Logger::log()  << "NULL";
        }
        RTT::Logger::log() << RTT::endlog();
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
    template <class T>
    ACES::Credentials<T>* Protocol::parseHWInput(
                       ACES::Message* c) {}
*/
    template <class T>
    State<T>::State(std::string n,
      ACES::Credentials<T>* c,
      int pri, int UpdateFreq)
      : ACES::State<T>(n, c, pri, UpdateFreq)
    {}

    template <class T>
    State<T>::State(std::string pname, std::string cname,
      int pri, int UpdateFreq,
      float z, float rot)
        : ACES::State<T>(pname,
                          new Credentials<T>(cname,z,rot),
                          pri, UpdateFreq)
    {}

    template <class T>
    State<T>::State(ACES::taskCfg cfg, Credentials<T>* c, T ic)
     :ACES::State<T>(cfg, (ACES::Credentials<T>*) c, ic)
    {}
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
