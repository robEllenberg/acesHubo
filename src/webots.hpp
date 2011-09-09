/* 
    Conductor: High Degree of Freedom Robot Controller Framework
    Copyright (C) 2010, 2011 Robert Sherbert
    bob.sherbert@gmail.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    If you are interested in licensing this software for commercial purposes
    please contact the author. The software can be licensed to you under
    non-free license terms by the copyright holder.

    As a courtesy to the author, and in the spirit of fair attribution, you are
    asked to cite the following paper if any work based upon or utilizing this
    framework is published in the scientific literature: 
    Sherbert, Robert M. and Oh, Paul Y. "Conductor: A Controller Development
    Framework for High Degree of Freedom Systems." Intelligent Robots and
    Systems (IROS), 2011 IEEE/RSJ International Conference on. 
*/

#ifndef ACES_WEBOTS_HPP
#define ACES_WEBOTS_HPP

#include <map>
#include <string>

#include <rtt/TaskContext.hpp>
#include <rtt/Logger.hpp>
#include <rtt/Activity.hpp>
#include <rtt/Port.hpp>
#include <rtt/os/TimeService.hpp>

#include "protocol.hpp"
#include "state/state.hpp"
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
    class Hardware : public ACES::Hardware<float> {
        public:
            Hardware(std::string cfg, std::string args);
            bool startHook();
            void updateHook();
            virtual bool txBus(ACES::Message<float>* m);

            void step(int time=0);
            //void stepRequest( std::map<std::string, void*>* );
            virtual bool subscribeController(RTT::TaskContext* c);

            //std::deque< ACES::Word<T> > usQueue;
            //RTT::OS::Mutex usqGuard; 
            RTT::InputPort< std::map<std::string, void*>* >stepRequest;
        private:
            RTT::os::TimeService::ticks lastStep;
    };

    class Credentials : public ACES::Credentials {
    //TODO - Lookup appropriate WbDeviceTag elements when the cred
    //is created, saves lots of lookup processing.
        public:
            Credentials(COMP_TYPE devID, std::string wb_id);
            static Credentials* makeCredentials(COMP_TYPE devID,
                                                std::string name);
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

    //template<class P> 
    class JointDevice : public ACES::Device<float, float> {
        public:
            JointDevice(std::string config, std::string args);
            //!Used by the HW to interact w/Webots for the refresh
            //void interpretResult(ACES::ProtoResult* rx);
            bool startHook();
            void stopHook();
    };

    bool setJoint(JointCredentials* j, ACES::Word<float>* s);
    float refreshJoint(JointCredentials* j);

    class TripletDevice : public ACES::Device<float, float> {
        public:
            TripletDevice(std::string config, std::string args,
                          COMP_TYPE devID);
            //virtual const double* getTriplet(WbDeviceTag tag) = 0;
            //virtual std::list<ACES::ProtoResult*> processUSQueue(
            //    ACES::Word< list<float> >* usIn);
            void (*wb_start_fun)(WbDeviceTag tag, int ms);
            void (*wb_stop_fun)(WbDeviceTag tag);
            bool startHook();
            void stopHook();
    };

    std::vector<float>* refreshTriplet(Credentials* c);

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
    class Protocol : public ACES::Protocol<float,float> {
        public:
            Protocol(std::string cfg, std::string args);
    };
}    

#endif
