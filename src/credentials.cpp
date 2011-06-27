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

#include "credentials.hpp"
namespace ACES {
    Credentials::Credentials(int id){
        devID = id;
    }

    int Credentials::getDevID(){
        return devID;
    }

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
