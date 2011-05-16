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

#include "robotisDS.hpp"

namespace Robotis {

    RobotisPacket::RobotisPacket(){
        //counter = 0;
        id = 0;
        len = 0;
        error = 0;
        checksum = 0;
        parameters = new std::deque<unsigned char>();
    }

    void RobotisPacket::setID(unsigned char i){
        id = i;
    }

    void RobotisPacket::setLen(unsigned char l){
        len = l;
    }

    void RobotisPacket::setError(unsigned char err){
        error = err;
    }

    void RobotisPacket::setChecksum(unsigned char check){
        checksum = check;
    }

    void RobotisPacket::setInst(INST inst){
        instruct = inst;
    }

    unsigned char RobotisPacket::getID(){
        return id;
    }

    unsigned char RobotisPacket::getLen(){
        return len;
    }

    unsigned char RobotisPacket::getError(){
        return error;
    }

    unsigned char RobotisPacket::getChecksum(){
        return checksum;
    }

    INST RobotisPacket::getInst(){
        return instruct;
    }

    void RobotisPacket::printme(){
        RTT::Logger::log() << "ID: " << (int)id << ", Len: " << (int)len
                           << ", Error: " << (int)error 
                           << ", Check: " << (int)checksum;
        RTT::Logger::log() << ", Parameters: ";
        for(std::deque<unsigned char>::iterator it = parameters->begin();
            it != parameters->end(); it++)
        {
            RTT::Logger::log() <<  (int)(*it) << ", ";
        }
        RTT::Logger::log() << RTT::endlog();
    }
};
