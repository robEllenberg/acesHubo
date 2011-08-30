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

#include <fstream>
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


//Set to 1 for file I/O mode, and 0 for the actual PCM3680 driver
#define TESTMODE 1
#define RAD2DEG (180.0/3.14159)

namespace Hubo{
    const int ctrlSize = 5;
    class MotorCredentials : public ACES::Credentials {
        friend class MotorDevice;
        public:
            MotorCredentials(int board, int channels);
            static ACES::Credentials* makeCredentials(std::string args);
            void printme();

            int getChannels();
            //float getPPR(int chan);
            float getDirection(int chan);
            unsigned int getEncoderSize(int chan);
            //int getOffsetPulse(int chan);
            //int getRevOffset(int chan);
            //bool getCCW(int chan);
            unsigned int getHarmonic(int chan);
            float getGearRatio(int chan);
            int getZeroTicks(int chan);
            bool getZeroCCW(int chan);
        protected:
            //bool setPPR(int chan, float proposedPPR);
            bool setDirection(int chan, float dir);
            bool setGearRatio(int chan, int drive, int driven);
            bool setEncoderSize(int chan, int size);
            //bool setOffsetPulse(int chan, int offset);
            //bool setRevOffset(int chan, int offset);
            bool setHarmonic(int chan, int harmonic);
            bool setZero(int chan, int ticks, bool ccw);
            //bool setCCW(int chan, bool CCW);
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
            int zeroTicks[ctrlSize];
            bool zeroCCW[ctrlSize];
            //int offsetPulse[ctrlSize];
            //int revOffset[ctrlSize];
            //bool CCW[ctrlSize];
            //TODO - Do we need to save the 'motor number' for each channel? 
    };

    const int senseSize = 4;
    class SensorCredentials : public ACES::Credentials
    {
        friend class SensorDevice;
        public:
            SensorCredentials(int boardID, int chan);
            static ACES::Credentials* makeCredentials(std::string args);
            float getDirection(int chan);
            float getScale(int chan);
            int getChannels();
            void printme();
        protected:
            bool setDirection(int chan, float direction);
            bool setScale(int chan, float scale);
        private:
            bool checkChannel(int chan);
            int channels;
            float direction[senseSize]; //! +/-1.0, use to force sign to convention
            float scale[senseSize];//! Arbitrary (multiplicative) scaling const
    };

    #if TESTMODE == 1
    const int canBuffSize = 1; //!The number of lines to load from the log each
                               //tick
    #else
    const int canBuffSize = 100; //!The maximum number of packets we can pull 
                                 // from the FIFO buffer on each tick
    #endif
    canmsg_t rxBuffer[canBuffSize]; 
    class CANHardware : public ACES::Hardware<canmsg_t*>
    {
        public:
            CANHardware(std::string cfg, std::string args);
            virtual bool txBus(ACES::Message<canmsg_t*>* m);
            virtual void rxBus(int size=0);
            bool startHook();
            void stopHook();
            //virtual bool processUS(ACES::Word<canmsg_t*>* w);
            bool genPacket(int ID, int len, std::string packet);
                           
        protected:
            std::string fd;
            int rate;
            #if TESTMODE == 1
                std::ifstream ichannel;   //! File descripted for input (offline only)
                RTT::os::TimeService::ticks beginning;
            #endif
                int channel;    //! File descriptor for the CAN access node
                                // Used for output in offline mode
            RTT::OutputPort< ACES::Message<canmsg_t*>* > txDSLoop;
    };

    class Protocol : public ACES::Protocol<canmsg_t*, canMsg> {
        public:
            Protocol(std::string cfg, std::string args);
            static ACES::Credentials* credFromPacket(canMsg& c);
            virtual ACES::Message<canmsg_t*>*
                      processDS(ACES::Word<canMsg>*);
            virtual ACES::Word<canMsg>* processUS(ACES::Word<canmsg_t*>* usIn);
            //Helper functions
            bool offsetRange(huboCanType t, int lineID);
            unsigned long assemble2Byte(unsigned char lsb, unsigned char msb);
    };

    class HuboDevice : public ACES::Device<float, canMsg>{
        public: 
            HuboDevice(std::string cfg, std::string args);
            void processUS_NAME_RXDF(ACES::Word<canMsg>* msg);
            void processUS_STAT_RXDF(ACES::Word<canMsg>* msg);
        protected:
            ACES::Word<canMsg>* buildWord(canMsg c, int channel);
    };

    class MotorDevice : public HuboDevice{
        public:
            MotorDevice(std::string cfg, std::string args);
            virtual ACES::Word<canMsg>* processDS(ACES::Word<float>*);
            virtual ACES::Word<float>* processUS(ACES::Word<canMsg>*);
            void processUS_SENSOR_FT_RXDF(ACES::Word<canMsg>* msg);
            void processUS_AD_RXDF(ACES::Word<canMsg>* msg);

            int getChannels();
            //User facing (local) Configuration functions
            bool setDirection(int channel, float direction);
            bool setGearRatio(int chan, int drive, int driven);
            bool setEncoderSize(int chan, int size);
            //bool setOffsetPulse(int chan, int offset);
            //bool setRevOffset(int chan, int offset);
            //bool setCCW(int chan, bool CCW);
            bool setZero(int chan, int ticks, bool ccw);
            bool programZero(int chan);
            //bool setCalibrate(int channel);
            bool setHIPenable();
            bool setRunCmd();
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
            //Helpers for buildCalibratePulse
            //long calPulse2Chan(int c);
            //long calPulse3Chan(int c);
            //Functions for generating the different types of configuration
            //packets.
            canMsg buildSetPacket();
            canMsg buildGainPacket(cmdType type, int Kp, int Ki, int Kd);
            canMsg buildZeroPacket(int c, int ticks, bool ccw);
            //canMsg buildCalibratePulse(int c);
            canMsg buildHIPpacket();
            canMsg buildRunCmdPacket();
        private:
            float setPoint[ctrlSize];
            bool trigger[ctrlSize]; /*! Indicates which channels have recieved
                                     * information since the last trigger */
            bool instantTrigger;
    };

    class SensorDevice : public HuboDevice{
        public:
            SensorDevice(std::string cfg, std::string args);
            ACES::Word<float>* processUS(ACES::Word<canMsg>* w);
            virtual ACES::Word<canMsg>* processDS(ACES::Word<float>*);
            void processUS_SENSOR_FT_RXDF(ACES::Word<canMsg>* msg);
            void processUS_SENSOR_AD_RXDF(ACES::Word<canMsg>* msg);

            canMsg buildRefreshPacket();
            bool setDirection(int chan, float direction);
            bool setScale(int chan, float scale);
    };
};

#endif
