#ifndef ACES_LOGGER_HPP
#define ACES_LOGGER_HPP

#include <list>
#include <string>
#include <sstream>

#include <rtt/TaskContext.hpp>
#include <rtt/TimeService.hpp>
#include <rtt/Time.hpp>
#include <boost/lexical_cast.hpp>

#include "state.hpp"
#include "dispatcher.hpp"
#include "taskcfg.hpp"

namespace ACES{
    class Dispatcher;
    class Logger : protected taskCfg,
                   public    RTT::TaskContext
    {
        public:
            Logger(std::string cfg, std::string args);

            bool configureHook();
            void updateHook();
            void cleanupHook();

            bool subscribeDispatcher(Dispatcher* d);
            bool addTrack(std::string track);
            bool addTracks(std::string tracks);
            virtual bool sample() = 0;
            //virtual bool store() = 0;

            std::list<ProtoState*> trackList;
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
            RTT::TimeService::ticks beginning;
            bool sample();
            //store();
    };
}

#endif
