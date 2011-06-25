/* AUTORIGHTS
    ACES: Abstracted Controls Engineering Software
    Copyright (C) 2010 Robert Sherbert
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
    please contact the author.
*/

#ifndef ACES_HUBO_HPP
#define ACES_HUBO_HPP

#include <sstream>
#include <iomanip>
#include <iostream>
#include <boost/array.hpp>

#include "../hardware.hpp"
#include "../device.hpp"
#include "../protocol.hpp"
#include "../message.hpp"
#include "../word.hpp"
#include "../credentials.hpp"

#include "huboCanDS.hpp"
#include "can4linux.h"  //Include the can4linux data structures

namespace Hubo{
    const int ctrlSize = 5;
    class Credentials : public ACES::Credentials {
        friend class MotorDevice;
        public:
            Credentials(int board, int channels);
            static Credentials* makeCredentials(std::string args);
            int getChannels();
            //float getPPR(int chan);
            float getDirection(int chan);
            unsigned int getEncoderSize(int chan);
            int getOffsetPulse(int chan);
            int getRevOffset(int chan);
            bool getCCW(int chan);
            unsigned int getHarmonic(int chan);
            float getGearRatio(int chan);
            void printme();
        protected:
            //bool setPPR(int chan, float proposedPPR);
            bool setDirection(int chan, float dir);
            bool setGearRatio(int chan, int drive, int driven);
            bool setEncoderSize(int chan, int size);
            bool setOffsetPulse(int chan, int offset);
            bool setRevOffset(int chan, int offset);
            bool setHarmonic(int chan, int harmonic);
            bool setCCW(int chan, bool CCW);
        private:
            //int boardNum;       //! Identifying number for the motor controller
            bool checkChannel(int chan);
            int channels;       //! Number of channels on the controller (1-3)
            //float PPR[ctrlSize];       //! Pulses per revolution (one per channel)
            float direction[ctrlSize]; //! +/-1 Direction of motor revolution (1/channel)
            unsigned int driveTeeth[ctrlSize];
            unsigned int drivenTeeth[ctrlSize];
            unsigned int encoderSize[ctrlSize];
            unsigned int harmonic[ctrlSize];
            int offsetPulse[ctrlSize];
            int revOffset[ctrlSize];
            bool CCW[ctrlSize];
            //TODO - Do we need to save the 'motor number' for each channel? 
    };

    class CANHardware : public ACES::Hardware<canmsg_t*>
    {
        public:
            CANHardware(std::string cfg, std::string args);
            virtual bool txBus(ACES::Message<canmsg_t*>* m);
            virtual void rxBus(int size=0);
            bool startHook();
            void stopHook();
            //virtual bool processUS(ACES::Word<canmsg_t*>* w);
            bool genPacket(int ID, int len, 
                           std::string lower, std::string upper);
        protected:
            std::string fd;
            int rate;
            int channel;    //! File descriptor for the CAN access node
            RTT::OutputPort< ACES::Message<canmsg_t*>* > txDSLoop;
    };

    class Protocol : public ACES::Protocol<canmsg_t*, canMsg> {
        public:
            Protocol(std::string cfg, std::string args);
            virtual ACES::Message<canmsg_t*>*
                      processDS(ACES::Word<canMsg>*);
    };

    class MotorDevice : public ACES::Device<float, canMsg>{
        public:
            MotorDevice(std::string cfg, std::string args);
            virtual ACES::Word<canMsg>* processDS(ACES::Word<float>*);

            int getChannels();
            //User facing (local) Configuration functions
            //bool setPPR(int channel, float PPR);
            bool setDirection(int channel, float direction);
            bool setGearRatio(int chan, int drive, int driven);
            bool setEncoderSize(int chan, int size);
            bool setOffsetPulse(int chan, int offset);
            bool setRevOffset(int chan, int offset);
            bool setCCW(int chan, bool CCW);
            bool setCalibrate(int channel);
            bool setHarmonic(int chan, int harmonic);
            //User facing (remote) configuration functions
            bool setGains(std::string type, int channel,
                          int Kp, int Ki, int Kd);
            bool setSetPoint(int channel, float sp,
                             bool instantTrigger=false);
            //Helper functions for processing new set points and config
            //information
            bool applySetPoint(int channel, float sp, bool instantTrigger);
            bool triggersSet();
            void clearTrigger();
            ACES::Word<canMsg>* buildWord(canMsg c, int channel);
            //Helpers for buildCalibratePulse
            long calPulse2Chan(int c);
            long calPulse3Chan(int c);
            //Functions for generating the different types of configuration
            //packets.
            canMsg buildSetPacket();
            canMsg buildGainPacket(cmdType type, int Kp, int Ki, int Kd);
            canMsg buildCalibratePulse(int c);
        private:
            float setPoint[ctrlSize];
            bool trigger[ctrlSize]; //! Indicates which channels have recieved
                             // information since the last trigger
            bool instantTrigger;
    };

    /*
    class SensorDevice : public ACES::Device<float, canMsg>{
        public:
            SensorDevice(std::string cfg, std::string args);
            virtual ACES::Word<canMsg>* processDS(ACES::Word<float>*);
            canMsg buildRefreshPacket();
    };
    */
};

#endif
