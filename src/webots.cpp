#include "webots.hpp"

namespace Webots {

    int JointCredentials::idCount = 0;

    Hardware::Hardware(std::string cfg, std::string args)
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
        //We want a null action on the tick, so we override this to nothing
        //and use step(), to manually advance the clock.
    }

    void Hardware::step(int time){
        ACES::Hardware::updateHook();
        wb_robot_step(time);
    }

    bool Hardware::txBus(ACES::Message* m){
        for(std::list<ACES::Goal*>::iterator it = m->goalList.begin();
          it != m->goalList.end(); it++)
        {
            ACES::Goal* g = *it;
            void* result = NULL;
            Credentials* c = (Credentials*)(g->cred);

            switch( c->devID ){
                case(JOINT):{
                    switch(g->mode){
                        case(ACES::REFRESH):
                            result = JointDevice::
                                        refresh((JointCredentials*)c);
                            break;
                        case(ACES::SET):
                            JointDevice::set((JointCredentials*)c, g);
                            break; 
                        default:
                            break;
                    }
                }; break;

                case(IMU):{
                    switch(g->mode){
                        case(ACES::REFRESH):
                            result = IMUDevice::
                                        refresh((IMUCredentials*)c);
                            break;
                        default:
                            break;
                    }
                }; break;

                case(GPS):{
                    switch(g->mode){
                        case(ACES::REFRESH):
                            result = GPSDevice::
                                      refresh((GPSCredentials*)c);
                            break;
                        default:
                            break;
                    }
                }; break;

                default:
                    break;
            }
        
            //Place the result on the container & send it back if appropriate
            if(g->mode == ACES::REFRESH){
                //TODO - This kind of bypasses the physical HW Rx
                //structure, perhaps we should change that somehow
                assert(result); //Make sure we're actually sending
                                //something
                g->data = result;
                ACES::ProtoWord* w =
                    (ACES::ProtoWord*)(new ACES::Word<ACES::Goal*>(g));
                { RTT::OS::MutexLock lock(usqGuard);
                  usQueue.push_back(w);
                }
            }
        }
    }

    bool Hardware::subscribeController(ACES::Controller* c){
        this->connectPeers( (RTT::TaskContext*) c);
        RTT::Handle h = c->events()->setupConnection("applyStateVector")
                .callback( this, &Hardware::stepRequest,
                           this->engine()->events() ).handle();
        if(!h.ready() ){
            return false;
        }
        h.connect();
        if(!h.connected() ){
            return false;
        }
        return true;
    }

    void Hardware::stepRequest( std::map<std::string, void*>* ){
        //TODO - This will advance the simulation before the 
        //new state information is processed
        step();
    }

    Credentials::Credentials(COMP_TYPE id, std::string wb_id)
    : ACES::Credentials((int)id)
    {
        wb_device_id = wb_id;
    }

    bool Credentials::operator==(ACES::Credentials& other){
        if(not ACES::Credentials::operator==(other) ){
            return false;
        }
        if(dynamic_cast<Credentials*>(&other) ){
            Credentials* c = (Credentials*)&other;
            if(c->wb_device_id == wb_device_id){
                return true;
            }
            else{
                return false;
            }
        }

        return false;
    }

    void Credentials::printme(){
        ACES::Credentials::printme();
        RTT::Logger::log() << "(Webots) Credentials: wb_dev_id="
                           << wb_device_id << RTT::endlog();
    }

    void JointCredentials::printme(){
        Credentials::printme();
        RTT::Logger::log() << "(Joint) Credentials: zero=" << zero
                           << " direction=" << direction 
                           << RTT::endlog();
    }

    JointCredentials::JointCredentials(std::string wb_id, float z, float dir)
      : Credentials(JOINT, wb_id)
    {
        //wb_device_id = wb_id;
        zero = z;
        direction = dir;
    }

    JointCredentials* JointCredentials::makeJointCredentials(std::string args)
    {
        std::istringstream s1(args);
        float z, d;
        std::string id;
        s1 >> id >> z >> d;
        JointCredentials *j = new JointCredentials(id, z, d);
        return j;
    }

    GPSCredentials* GPSCredentials::makeGPSCredentials(std::string args)
    {
        std::istringstream s1(args);
        std::string name;
        s1 >> name;
        GPSCredentials *g = new GPSCredentials(name);
        return g;
    }

    GPSCredentials::GPSCredentials(std::string wb_id)
      : Credentials(GPS, wb_id)
    {}

    JointDevice::JointDevice(std::string cfg, std::string args)
      : ACES::Device(cfg)
    {
        //std::string rargs = args + (std::string)" " + name;
        //RTT::Logger::log() << rargs << RTT::endlog();
        credentials = (Credentials*) JointCredentials::makeJointCredentials(args);
        //credentials.devName = name;
    }

    void* JointDevice::refresh(JointCredentials* j){
        float* res  = new float;
        WbDeviceTag tag =
                wb_robot_get_device( (j->wb_device_id).c_str() );

        *res = wb_servo_get_position(tag);

        //Apply the appropriate sign to the result, so it is
        //consistant w/convention
        *res = (*res) * (j->direction);
    
        return (void*)res;
     }

     bool JointDevice::set(JointCredentials* j,
                                       ACES::Goal* g){
        std::string jid = j->wb_device_id;
        WbDeviceTag joint = wb_robot_get_device(jid.c_str());

        //Pull the seek value out of the credentials 
        float* tp = (float*)(g->data);
        float target = *tp;
        float angle = j->direction
                       * (target - j->zero);

        //wb_servo_set_position(joint, 3.14159/180.*angle);
        wb_servo_set_position(joint, angle);
        return true;
     }
    
    bool JointDevice::startHook(){
        Credentials* c = (Credentials*)credentials;
        WbDeviceTag tag = wb_robot_get_device( (c->wb_device_id).c_str() );
        //TODO - Magic Number removal (Samples every 8 ms)
        wb_servo_enable_position(tag, 8); 
        
        return true;
    }
    
    void JointDevice::stopHook(){
        Credentials* c = (Credentials*)credentials;
        WbDeviceTag tag = wb_robot_get_device( (c->wb_device_id).c_str() );
        wb_servo_disable_position(tag); 
    }

    void* IMUDevice::refresh(IMUCredentials* j)
    {
        WbDeviceTag tag =
                wb_robot_get_device( (j->wb_device_id).c_str() );
        const double *val = wb_accelerometer_get_values(tag);
        //We must clone the vector because Webots owns the memory
        //and will yank the values out from under us at the next
        //timestep
        std::vector<double>* res = new std::vector<double>(3);
        (*res)[0] = val[0];
        (*res)[1] = val[1];
        (*res)[2] = val[2];
        return (void*)res;
     }

    GPSDevice::GPSDevice(std::string cfg, std::string args)
      : ACES::Device(cfg)
    {
        //RTT::Logger::log() << rargs << RTT::endlog();
        credentials = (ACES::Credentials*)( new GPSCredentials(args) );
    }

    void* GPSDevice::refresh(GPSCredentials* g){
        WbDeviceTag tag =
                wb_robot_get_device( (g->wb_device_id).c_str() );

        const double *val = wb_gps_get_values(tag);
        //We must clone the vector because Webots owns the memory
        //and will yank the values out from under us at the next
        //timestep
        std::vector<double>* res = new std::vector<double>(3);
        (*res)[0] = val[0];
        (*res)[1] = val[1];
        (*res)[2] = val[2];
        //RTT::Logger::log() << (*res)[0] << " " << (*res)[1]
        //                   << " " << (*res)[2] << RTT::endlog();
        return (void*)res;
    }

    bool GPSDevice::startHook(){
        Credentials* c = (Credentials*)credentials;
        WbDeviceTag tag = wb_robot_get_device( (c->wb_device_id).c_str() );
        //TODO - Magic Number removal (Samples every 8 ms)
        wb_gps_enable(tag, 8);
        
        return true;
    }

    void GPSDevice::stopHook(){
        Credentials* c = (Credentials*)credentials;
        WbDeviceTag tag = wb_robot_get_device( (c->wb_device_id).c_str() );
        wb_gps_disable(tag); 
    }

    //!Override the default USQueue processor in order to split the
    //!three data elements out of the GPS node's return packet
    std::list<ACES::ProtoResult*> GPSDevice::processUSQueue(){
        //TODO - Provide an implementation that doesn't triple up the
        //requests into webots
        RTT::Logger::log() << "GPS  Dev US override" << RTT::endlog();
        ACES::ProtoResult* p = NULL;
        { RTT::OS::MutexLock lock(usqGuard);
          p = usQueue.front();
          usQueue.pop_front();
        }
        ACES::Goal* g = ( (ACES::Result<ACES::Goal*>*)p)->result;
        std::vector<double>* response = (std::vector<double>*)g->data;

        std::list<ACES::ProtoResult*> pr_list;
        for(int i =0; i < 3; i++){
            float* f = new float((*response)[i]);
            ACES::Result<void*>* r = new ACES::Result<void*>(
                                     (void*)f, g->cred, X+i);
            pr_list.push_back( (ACES::ProtoResult*)r );
        }
        return pr_list;
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
      : ACES::ScriptCtrl(cfg, args)
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

/*
#include <ocl/ComponentLoader.hpp>
ORO_CREATE_COMPONENT_TYPE()
ORO_LIST_COMPONENT_TYPE( Webots::Hardware )
ORO_LIST_COMPONENT_TYPE( Webots::Device )
ORO_LIST_COMPONENT_TYPE( Webots::Protocol )
ORO_LIST_COMPONENT_TYPE( Webots::ScriptCtrl )
ORO_LIST_COMPONENT_TYPE( Webots::ArmCtrl )
*/
