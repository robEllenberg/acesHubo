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

#ifndef ACES_LOGGER_HPP
#define ACES_LOGGER_HPP

#include <list>
#include <string>
#include <sstream>

#include <rtt/TaskContext.hpp>
#include <rtt/os/TimeService.hpp>
//#include <rtt/Time.hpp>
#include <boost/lexical_cast.hpp>

#include "state.hpp"
#include "dispatcher.hpp"
#include "taskcfg.hpp"

namespace ACES{
    class Dispatcher;

    class Track {
        public:
            Track(ProtoState* s, std::string att);
            ProtoState* getState();
            std::string getAttribute();
        private:
            ProtoState* state;
            std::string attribute;
    };

    class Logger : protected taskCfg,
                   public    RTT::TaskContext
    {
        public:
            Logger(std::string cfg, std::string args);

            void updateHook();

            bool subscribeDispatcher(Dispatcher* d);
            bool addTrack(ProtoState* s, std::string attribute);
            //bool addTracks(std::string tracks);
            virtual bool sample() = 0;
            //virtual bool store() = 0;

            std::list<Track> trackList;
            Dispatcher* dispatch;
    };

    class FileLog : public Logger
    {
        public:
            FileLog(std::string cfg, std::string args);

            bool startHook();
            void stopHook();

            std::string filename;
            std::ofstream outFile;
            RTT::os::TimeService::ticks beginning;
            bool sample();
            //store();
    };
}

#endif
