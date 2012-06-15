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

#include "credentials.hpp"
namespace ACES {
    Credentials::Credentials(int id){
        devID = id;
    }

    int Credentials::getDevID(){
        return devID;
    }

    /*! \param args a string representation of the device ID e.g. "23" */
    Credentials::Credentials(std::string args){
        std::istringstream s1(args, std::istringstream::in);
        int idnum;
        s1 >> idnum;
        this->devID = idnum;
    }

    bool Credentials::operator==(Credentials& other){
        if(devID == other.devID){
            return true;
        }
        else{
            return false;
        }
    }

    /*! Debugging function for printing the device ID to the logging stream. */
    void Credentials::printme(){
        RTT::Logger::log() << "(Base) Credentials: devID=" << devID << RTT::endlog();
    }

    Credentials* Credentials::makeCredentials(std::string args){
        ACES::Credentials* c = new ACES::Credentials(args);
        return c;
    }

    Credentials* Credentials::makeCredentials(int newid){
        ACES::Credentials* c = new ACES::Credentials(newid);
        return c;
    }
}
