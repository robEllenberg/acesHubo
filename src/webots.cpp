#include "webots.hpp"

namespace Webots {

    int Credentials::idCount = 0;

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
        it != m->goalList.end();
        it++){
        ACES::Goal* g = *it;
        switch( g->mode ){
            case (ACES::REFRESH):{
                //Vessel for returning the information we find
                //g->printme();
                float* result = new float;

                //Lookup the identifier for the component of interest
                Credentials* c = (Credentials*)(g->cred);
                WbDeviceTag tag =
                    wb_robot_get_device( (c->wb_device_id).c_str() );

                //Different commands are needed depending on the
                //type of device we're looking up. Find the appropriate
                //result for this propID
                switch (c->devID){
                    case (ACES::JOINT):{
                        *result = wb_servo_get_position(tag);
                     }
                     break;

                     case (ACES::IMU):{
                        double *val = wb_gps_get_values(tag);
                        switch (c->nodeID){
                            case(ACES::X):
                                *result = val[0];
                            break;

                            case(ACES::Y):
                                *result = val[1];
                            break;

                            case(ACES::Z):
                                *result = val[2];
                            break;

                            default:
                                *result = 0.0;
                            break;
                        }
                     }
                     break;

                     default:
                        *result = 0;
                     break;
                }

                //Apply the appropriate sign to the result, so it is
                //consistant w/convention
                float direction = (*c).direction;
                *result = (*result) * direction;

                //Place the result on the container & send it back
                //TODO - This kind of bypasses the physical HW Rx
                //structure, perhaps we should change that somehow
                g->data = (void*)result;
                ACES::ProtoWord* w =
                    (ACES::ProtoWord*)(new ACES::Word<ACES::Goal*>(g));
                { RTT::OS::MutexLock lock(usqGuard);
                  usQueue.push_back(w);
                }
                return true;
             }
             break;

            case (ACES::SET):{
                Credentials* c = (Credentials*)(g->cred);
                std::string jid = (*c).wb_device_id;

                //Pull the seek value out of the SValue object
                float* tp = (float*)(g->data);
                float target = *tp;
                float angle = (*c).direction
                               * (target - (*c).zero);

                WbDeviceTag joint = wb_robot_get_device(jid.c_str());
                //wb_servo_set_position(joint, 3.14159/180.*angle);
                wb_servo_set_position(joint, angle);
                return true;
             }
             break;

            default:
             break;
        }
        return false;
        //m->printme();
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

    Credentials::Credentials(Credentials* c)
    : ACES::Credentials(c->devID)
    {
        assign(c->wb_device_id, /*c->devName,*/ c->zero, c->direction);
    }

    void Credentials::assign(std::string id_str, //std::string devname,
                           float z, float dir)
    {
        wb_device_id = id_str;
        zero = z;
        direction = dir;
        //devName = devname;
    }  

    Credentials::Credentials(std::string id_str, //std::string devname,
     float z, float dir) : ACES::Credentials(0)
    {
        //devID = ++Credentials::idCount; 
        devID = ACES::JOINT;

        assign(id_str, /*devname,*/ z, dir);
    }  

    void Credentials::printme(){
        RTT::Logger::log() << "Webots: ID= " << wb_device_id;
        RTT::Logger::log() << " Zero= " << zero;
        RTT::Logger::log() << " Direction= " << direction;
        //RTT::Logger::log() << " Device Name= " << devName;
        RTT::Logger::log() << RTT::endlog();
    }

    Credentials::Credentials(std::string args)
      : ACES::Credentials(0)
    {
        //devID = ++Credentials::idCount; 
        devID = ACES::JOINT;

        std::istringstream s1(args);
        float z, d;
        std::string id/*, dname*/;
        s1 >> id >> z >> d /*>> dname*/;
        assign(id, /*dname,*/ z, d);
    }

    bool Credentials::compare(ACES::Credentials* cred){
        //Cast to a webots cred from a generic one
        Credentials* wcred = (Credentials*)cred;
        bool same = (wb_device_id == wcred->wb_device_id);        
        //same &= (zero == wcred->zero);
        //same &= (direction;
        return same;
    }

    JointDevice::Device(std::string cfg, std::string args)
      : ACES::Device(cfg)
    {
        //std::string rargs = args + (std::string)" " + name;
        //RTT::Logger::log() << rargs << RTT::endlog();
        credentials = (ACES::Credentials*)( new Credentials(args) );
        //credentials.devName = name;
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

    GPSDevice::Device(std::string cfg, std::string args)
      : ACES::Device(cfg)
    {
        //RTT::Logger::log() << rargs << RTT::endlog();
        credentials = (ACES::Credentials*)( new GPSCredentials(args) );
    }

    bool GPSDevice::startHook(){
        Credentials* c = (Credentials*)credentials;
        WbDeviceTag tag = wb_robot_get_device( (c->wb_device_id).c_str() );
        //TODO - Magic Number removal (Samples every 8 ms)
        wb_gps_enable(tag, 8);
        
        return true;
    }

    GPSCredentials::GPSCredentials(std::string args){
        devID = ACES::IMU;
    }


    bool GPSCredentials::compare(ACES::Credentials* cred){
        //Cast to a webots cred from a generic one
        GPSCredentials* wcred = (Credentials*)cred;
        bool same = (axis == wcred->axis);        
        //same &= (zero == wcred->zero);
        //same &= (direction;
        return same;
    }

    void GPSDevice::stopHook(){
        Credentials* c = (Credentials*)credentials;
        WbDeviceTag tag = wb_robot_get_device( (c->wb_device_id).c_str() );
        wb_gps_disable(tag); 
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
