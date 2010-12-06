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

#include "logger.hpp"

namespace ACES {
    Track::Track(ProtoState* s, std::string att){
        state = s;
        attribute = att;
    }

    ProtoState* Track::getState(){
        return state;
    }

    std::string Track::getAttribute(){
        return attribute;
    }

    Logger::Logger(std::string cfg, std::string args)
      : taskCfg(cfg),
        TaskContext(name)
    {
        this->setActivity(
            new RTT::Activity( priority, 1.0/freq, 0, name )
        );
    }

    void Logger::updateHook()
    {
        sample();
        //store();
    }

    bool Logger::subscribeDispatcher(Dispatcher* d){
        dispatch = d;
        return true;
    }

    bool Logger::addTrack(ProtoState* s, std::string attribute){
        //ProtoState* s = (ProtoState*)(dispatch->getPeer(track));
        if(s){
            Track newtrack(s, attribute);
            trackList.push_back(newtrack);
            return true;
        }
        return false;
    }

    FileLog::FileLog(std::string cfg, std::string args) 
      : Logger(cfg, args)
    {
        filename = args;
    }
    
    bool FileLog::startHook(){
        outFile.open((const char*)filename.c_str());
        beginning = RTT::os::TimeService::Instance()->getTicks();
        outFile << "0 ";
        for(std::list<Track>::iterator it = trackList.begin();
             it != trackList.end(); it++)
        {
            outFile << ((*it).getState())->Name() << "("
                    << (*it).getAttribute() << ") ";
        }
        outFile << std::endl;
        return true;
    }

    void FileLog::stopHook(){
        outFile.close();
    }

    bool FileLog::sample(){
        RTT::Seconds sampleTime =
            RTT::os::TimeService::Instance()->secondsSince(beginning);

        outFile << sampleTime << " ";
        for(std::list<Track>::iterator it = trackList.begin();
             it != trackList.end(); it++)
        {
            RTT::TaskContext* t = (TaskContext*)((*it).getState());
            RTT::Attribute<float> a =
                t->attributes()->getAttribute((*it).getAttribute());
            outFile << a.get() << " ";
        }
        outFile << std::endl;
        
        return true;
    }
}
