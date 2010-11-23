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
