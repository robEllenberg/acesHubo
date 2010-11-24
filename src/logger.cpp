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
