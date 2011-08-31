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

#include "taskcfg.hpp"
namespace ACES{

    /*
    taskCfg::taskCfg(){
    name = "";
    priority = 10;
    freq = 1;
    }
    */

    periodDriverMgr ACESTask::pdm; 

    ACESTask::ACESTask(std::string cfg)
      : taskCfg(cfg),
        RTT::TaskContext(name)
    {
        //Create the event driven task activity
        setActivity( new RTT::Activity( priority) );

        this->ports()->addEventPort("PDP", periodTrigger).doc(
                               "Periodic Driver Port - Input");

        //Create the helper class to deliever the periodic prompts that drive
        //the periodic update behavior

        //TODO - Needs to be started/stopped by the same functions that
        //start/stop the task itself.
        //perDrv = new periodDriver(priority, freq, name);
        if(freq){
            perDrv = (ACESTask::pdm).getDriver(1.0/freq);
            subscribePeriodDriver(perDrv);
        }
    }

    void ACESTask::subscribePeriodDriver(periodDriver* p){
        //Connect to the owner task's period driver port
        RTT::ConnPolicy policy = RTT::ConnPolicy::data();
        RTT::base::PortInterface *myPort = NULL, *theirPort=NULL;

        theirPort = (RTT::base::PortInterface*)p->ports()->getPort("PDP");
        myPort = (RTT::base::PortInterface*)this->ports()->getPort("PDP");
        bool success = theirPort->connectTo(myPort, policy);
        assert(success);
    }

    taskCfg::taskCfg(std::string cfg){
        std::istringstream s1(cfg, std::istringstream::in);
        s1 >> name;
        s1 >> priority;
        s1 >> freq;
    }

    std::string taskCfg::Name(){
        return name;
    }

    periodDriver::periodDriver(int pri, float period, std::string name)
      :RTT::TaskContext(name + "_PeriodDriver")
    {
        priority = pri;
        
        this->ports()->addPort("PDP", periodTrigger).doc(
                               "Periodic Driver Port - Output");

        setActivity( new RTT::Activity( priority, period ) );
        start();
    }

    void periodDriver::updateHook(){
        periodTrigger.write(1);
    }
    
    periodDriverMgr::periodDriverMgr(){
    }

    periodDriver* periodDriverMgr::getDriver(double period){
        periodDriver *p = NULL;
        int priority = 99;

        map<float, periodDriver*>::iterator it = perDrvMap.find(period);
        if(it == perDrvMap.end()){
            p = new periodDriver(priority, period,
                        boost::lexical_cast<string>( period ) );
            perDrvMap[period] = p; 
        }
        else{
            p = it->second;
        }
        return p;
    }
}
