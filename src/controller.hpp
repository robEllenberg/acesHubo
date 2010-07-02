#ifndef ACES_CONTROLLER_HPP
#define ACES_CONTROLLER_HPP

#include <vector>
#include <map>
#include <iostream>

#include <rtt/TaskContext.hpp>
#include <rtt/Event.hpp>

#include "parameter.hpp"
#include "pvalue.hpp"

namespace ACES{

    class WbController : public RTT::TaskContext {
        public:
            WbController(std::string n,
              std::vector<ACES::Parameter*> pl, 
              const char* scriptFile, ACES::Hardware* hw_one,
              ACES::Dispatcher* dispatch,
              int pri, int UpdateFreq);

            std::vector<ACES::Parameter*> plist;
            std::map<std::string,
                     ACES::Parameter*> pmap;

            //    RTT::Command<bool(ACES::PValue*)> > pmap;
            //    RTT::DispatchInterface* > pmap;
            std::ifstream walkScript;
            std::map<std::string, ACES::PValue*>*
                getStateVector();
            //void applyStateVector(std::map<std::string,
            //    ACES::PValue*>& sv);
            std::map<std::string, ACES::PValue*>* stateVect;

            void updateHook();
            bool configureHook();
            bool startHook();
            void stopHook();
            void cleanupHook();

            //RTT::Method<void(void)> stepMethod;
            void step();
            RTT::Event<void(std::map<std::string, ACES::PValue*>*)>
                applyStateVector;

            std::string name;
            int frequency;
            int priority;
            std::list<ACES::Hardware*> hwlist;
            ACES::Dispatcher* dispatcher;
    };
}

#endif
