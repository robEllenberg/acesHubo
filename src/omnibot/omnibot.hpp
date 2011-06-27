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

#ifndef ACES_OMNIBOT_HPP
#define ACES_OMNIBOT_HPP

#include "../hardware.hpp"
#include "../device.hpp"
#include "../protocol.hpp"
#include "../message.hpp"
#include "../word.hpp"
#include "../credentials.hpp"

namespace Omnibot {

    typedef enum { GET_FIRMWARE = 0x1,
                   GET_ERROR,
                   GET_CONFIG_PARAM,
                   SET_CONFIG_PARAM,
                   M0_COAST = 0x6,
                   M1_COAST,
                   M0_FWD_SHORT,
                   M0_FWD_LONG,
                   M0_REV_SHORT,
                   M0_REV_LONG,
                   M1_FWD_SHORT,
                   M1_FWD_LONG,
                   M1_REV_SHORT,
                   M1_REV_LONG } PololuStates;

    typedef enum { PARAM_DEVID = 0,
                   PARAM_PWM,
                   PARAM_SHUT_ON_ERR,
                   PARAM_SER_TIMEOUT} PololuQuikParams;

    class PololuDS {
        public:
            PololuDS();
            void printme();

            void setID(unsigned char myid);
            void setCommand(unsigned char command);
            void setParameterID(unsigned char pid);
            void setParameterVal(unsigned char pval);

            unsigned char getID();
            unsigned char getCommand();
            unsigned char getParameterID();
            unsigned char getParameterVal();
        private:
            //! The ID of the controller this packet is headed towards
            unsigned char id; 
            //! The actual command being issued (M0 Fwd, M1 Rev, etc)
            unsigned char commandID;
            //! For get/set commands, the config parameter of interest
            unsigned char parameterID;
            //! The value to be assigned
            unsigned char parameterVal;
    };

    class Protocol : public ACES::Protocol<unsigned char, PololuDS> {
        public:
            Protocol(std::string cfg, std::string args); 
            ACES::Message<unsigned char>*
              processDS(ACES::Word<PololuDS>* w);
    };

    class Device : public ACES::Device<float, PololuDS> {
        public:
            Device(std::string config, std::string args);
            ACES::Word<PololuDS>* processDS(ACES::Word<float>*);
            void buildMotionCmdPacket(PololuDS &p, unsigned char nodeID,
                                      int data);
            void setID(int newID);
    };
};
#endif
