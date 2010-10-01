#ifndef ACES_WEBOTS_HPP
#define ACES_WEBOTS_HPP

#include <map>
#include <string>

#include <rtt/TaskContext.hpp>
#include <rtt/Logger.hpp>
#include <rtt/Activity.hpp>
#include <rtt/Ports.hpp>
#include <rtt/Event.hpp>
#include <rtt/Buffer.hpp>

#include "protocol.hpp"
#include "state.hpp"
#include "hardware.hpp"
#include "taskcfg.hpp"
#include "credentials.hpp"
#include "device.hpp"
#include "word.hpp"

enum COMP_TYPE { JOINT=1, GPS, IMU};
enum AXIS { X=1, Y, Z};

extern "C"{
    #include <webots/robot.h>
    #include <webots/servo.h>
    #include <webots/gps.h>
}

namespace Webots{
    class Hardware : public ACES::Hardware {
        public:
            Hardware(std::string cfg, std::string args);
            bool startHook();
            void updateHook();
            bool txBus(ACES::Message* m);

            void step(int time=32);
            void stepRequest( std::map<std::string, void*>* );
            bool subscribeController(ACES::Controller* c);
    };

    class Credentials : public ACES::Credentials {
    //TODO - Lookup appropriate WbDeviceTag elements when the cred
    //is created, saves lots of lookup processing.
        public:
            Credentials(COMP_TYPE id);
            std::string wb_device_id;
    };
   
    class JointCredentials : public Credentials {
        private:
            //void assign(std::string id_str, float z,
            //            float dir);
        public:
            //Credentials(Credentials* c);
            JointCredentials(std::string args);
            //Credentials(std::string id_str,
            //            float z, float dir);
            virtual bool operator==(const ACES::Credentials& other);

            void printme();

            float zero;
            float direction;
            //std::string devName;
            static int idCount;
    };

    class IMUCredentials : public Credentials {
    };

    class GPSCredentials : public Credentials {
        public:
            GPSCredentials(std::string args);
            int axis;
            bool operator==(const ACES::Credentials& cred);
    };

    class JointDevice : public ACES::Device {
        public:
            JointDevice(std::string config, std::string args);
            //!Used by the HW to interact w/Webots for the refresh
            static void* refresh(JointCredentials* j);
            static bool set(JointCredentials* j,
                                       ACES::Goal* g);
            //void interpretResult(ACES::ProtoResult* rx);
            bool startHook();
            void stopHook();
    };

    class IMUDevice : public ACES::Device {
        public:
            static void* refresh(IMUCredentials* j);
    };

    class GPSDevice : public ACES::Device {
        public:
            GPSDevice(std::string config, std::string args);
            //!Used by the HW to interact w/Webots for the refresh
            static void* refresh(GPSCredentials* g);
            //void interpretResult(ACES::ProtoResult* rx);
            bool startHook();
            void stopHook();
    };

    //Probably no point in having this - as it does not make any extensions yet
    class Protocol : public ACES::Protocol {
        public:
            Protocol(std::string cfg, std::string args);
    };

    class ScriptCtrl : public ACES::ScriptCtrl
    {
        public:
            ScriptCtrl(std::string config, std::string args);

            std::map<std::string, void*>* 
                    getStateVector(bool echo=0);
    };

    class ArmCtrl : public ACES::ScriptCtrl
    {
        public:
            ArmCtrl(std::string config, std::string args);
            std::map<std::string, void*>* 
                    getStateVector(bool echo=0);
    };
}    

#endif
