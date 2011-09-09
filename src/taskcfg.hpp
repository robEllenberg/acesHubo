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

#ifndef ACES_TASKCFG_HPP
#define ACES_TASKCFG_HPP

#include <string>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include <rtt/TaskContext.hpp>
#include <rtt/Activity.hpp>
#include <ocl/TimerComponent.hpp>

namespace ACES{

    class taskCfg {
        protected:
            std::string name; 
            double freq;
            int priority;
        public:
            //taskCfg();
            taskCfg(std::string cfg);
            std::string Name();
    };

    class periodDriver : public RTT::TaskContext
    {
        public:
            periodDriver(int pri, float period, std::string name);
            virtual void updateHook();
        private:
            int priority;
            float frequency;
            RTT::OutputPort<int> periodTrigger;
    };

    class periodDriverMgr
    {
        public:
            periodDriverMgr();
            periodDriver* getDriver(double period);
        private:
            std::map<float, periodDriver*> perDrvMap;
    };

    class ACESTask : public taskCfg,
                     public RTT::TaskContext
    {
        public:
            ACESTask(std::string cfg);
        private:
            void subscribePeriodDriver(periodDriver* p);

            static periodDriverMgr pdm;
            periodDriver* perDrv;
            RTT::InputPort<int> periodTrigger;
    };

}
#endif
