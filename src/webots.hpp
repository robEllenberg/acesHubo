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

enum COMP_TYPE { JOINT=5, GPS, ACCELEROMETER, GYRO, FORCE};
enum AXIS { X=1, Y, Z};

extern "C"{
    #include <webots/robot.h>
    #include <webots/servo.h>
    #include <webots/gps.h>
    #include <webots/gyro.h>
    #include <webots/touch_sensor.h>
    #include <webots/accelerometer.h>
}

namespace Webots{
    template <class T>
    class Hardware : public ACES::Hardware<T> {
        public:
            Hardware(std::string cfg, std::string args);
            bool startHook();
            void updateHook();
            bool txBus(ACES::Message<T>* m);

            void step(int time=32);
            void stepRequest( std::map<std::string, void*>* );
            bool subscribeController(ACES::Controller* c);

            //std::deque< ACES::Word<T> > usQueue;
            //RTT::OS::Mutex usqGuard; 
    };

    class Credentials : public ACES::Credentials {
    //TODO - Lookup appropriate WbDeviceTag elements when the cred
    //is created, saves lots of lookup processing.
        public:
            Credentials(COMP_TYPE devID, std::string wb_id);
            static Credentials* makeCredentials(COMP_TYPE devID, std::string name);
            std::string wb_device_id;
            virtual void printme();
            virtual bool operator==(ACES::Credentials& other);
    };
   
    class JointCredentials : public Credentials {
        private:
        public:
            JointCredentials(std::string wb_id, float zero, float dir);
            static JointCredentials* makeJointCredentials(std::string args);

            //virtual bool operator==(const ACES::Credentials& other);

            virtual void printme();

            float zero;
            float direction;
            static int idCount;
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

    class TripletDevice : public ACES::Device {
        public:
            TripletDevice(std::string config, std::string args, COMP_TYPE devID);
            static void* refresh(Credentials* c);
            //virtual const double* getTriplet(WbDeviceTag tag) = 0;
            virtual std::list<ACES::ProtoResult*> processUSQueue();
            void (*wb_start_fun)(WbDeviceTag tag, int ms);
            void (*wb_stop_fun)(WbDeviceTag tag);
            bool startHook();
            void stopHook();
    };

    class GPSDevice : public TripletDevice {
        public:
            GPSDevice(std::string config, std::string args);
            static const double* getTriplet(WbDeviceTag tag);
    };

    class AccelerometerDevice : public TripletDevice {
        public:
            AccelerometerDevice(std::string config, std::string args);
            static const double* getTriplet(WbDeviceTag tag);
    };

    class GyroscopeDevice : public TripletDevice {
        public:
            GyroscopeDevice(std::string config, std::string args);
            static const double* getTriplet(WbDeviceTag tag);
    };

    class ForceDevice : public TripletDevice {
        public:
            ForceDevice(std::string config, std::string args);
            static const double* getTriplet(WbDeviceTag tag);
    };

    //Probably no point in having this - as it does not make any extensions yet
    template <class T>
    class Protocol : public ACES::Protocol<T> {
        public:
            Protocol(std::string cfg, std::string args);
    };
}    

#endif
