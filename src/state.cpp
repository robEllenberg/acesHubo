#include "state.hpp"
namespace ACES {
    template <class T>
    State<T>::State(std::string n, Credentials<T>* c,
      int pri, int UpdateFreq, T val)
      : RTT::TaskContext(n),
        sendGoal("sendGoal")
    {
        name = n;
        credentials = c;
        frequency = UpdateFreq;
        priority = pri;
        value = val;

        this->events()->addEvent(&sendGoal, "sendGoal", "credentials",
            "credentials associated w/the goal");

        this->setActivity(
            new RTT::Activity( priority, 1.0/UpdateFreq, 0, n )
        );
    }

    template <class T>
    State<T>::State(taskCfg cfg, Credentials<T>* c, T ic) :
      RTT::TaskContext(cfg.name), sendGoal("sendGoal"){

        this->events()->addEvent(&sendGoal, "sendGoal", "credentials",
            "credentials associated w/the goal");

        priority = cfg.pri;
        frequency = cfg.freq;
        credentials = c;
        value = ic;

        this->setActivity(
            new RTT::Activity( priority, 1.0/frequency, 0, cfg.name )
        );
    }
    
    template <class T>
    void State<T>::printme(){
        RTT::Logger::log() << "State: " << this->name
        << ", (F:" << this->frequency << ", P:"
        << this->priority << ")" << std::endl;

        this->credentials->printme();
    }
    
    template <class T>
    bool State<T>::configureHook(){
        return true;
    }
    
    template <class T>
    bool State<T>::startHook(){
        return true;
    }
    
    template <class T>
    void State<T>::updateHook(){
        Credentials<T>* c = new Credentials<T>(credentials);
        sendGoal(c);
        //RTT::Logger::log() << "Update State "
        //<< this->name << RTT::Logger::endl;

    }
    
    template <class T>
    void State<T>::stopHook(){
    }
    
    template <class T>
    void State<T>::cleanupHook(){
    }

    template <class T>
    void State<T>::setGoal(std::map<std::string, void*>* p){
        std::map<std::string, void* >::iterator mypair;
        mypair = p->find( name );
        if(mypair != p->end() ){
            T* valp = (*mypair).second;
            T val = *valp;
            //Credentials<T>* c = (Credentials<T>*) credentials->credCopy(val);
            Credentials<T>* c = new Credentials<T>(this->credentials, val);
            
            //this->outport->Push(c);
            //printme();
            sendGoal((void*)c);
        }
    }
}
