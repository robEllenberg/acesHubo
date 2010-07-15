    template <class T>
    State<T>::State(std::string n, int pid,
      int pri, int UpdateFreq, T val)
      : ProtoState(n, pid, pri, UpdateFreq)
    {
        value = val;
    }

    template <class T>
    State<T>::State(taskCfg cfg, int pid, T ic) :
      ProtoState(cfg, pid){
        value = ic;
    }
    
    template <class T>
    void State<T>::printme(){
        RTT::Logger::log() << "State: " << this->name
        << ", (F:" << this->frequency << ", P:"
        << this->priority << "),"
        << "(Val = " << value << ")" << std::endl;

        this->credentials->printme();
    }

    

