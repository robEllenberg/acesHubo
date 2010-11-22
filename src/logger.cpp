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
/*
    bool Logger::addTracks(std::string tracks){
        std::istringstream s1(tracks, std::istringstream::in);

        s1.peek();
        while( ! s1.eof() ){
            std::string stringTemp;
            s1 >> stringTemp;
            addTrack(stringTemp);
        }
        return true;
    }
*/
/*
    bool Logger::sample(){
        for( std::list<ProtoState*>::iterator it = trackList.begin();
             it != trackList.end(); it++)
        {
            //Need some kind of variable/dynamic type container
            //or determination.
            //Do we want to be presenting state information back to
            //nodes on the OS? (probably)
            ProtoState* p = *it;

            //Downcasting our ProtoState to the specific State<T> type
            __typeof__(p) s = ( __typeof__(p) ) p;
            //Recover the appropriate attribute from the state
            __typeof__(s->value) v =
                s->attributes()->
                getAttribute< __typeof__(s->value.get()) >("value");
        }
        RTT::Seconds sampleTime = RTT::TimeService::Instance()->secondsSince(0);
    }
*/
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
