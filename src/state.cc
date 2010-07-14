    template <class T>
    State<T>::State(std::string n, Credentials* c,
      int pri, int UpdateFreq, T val)
      : ProtoState(n, c, pri, UpdateFreq)
    {
        value = val;
    }

    template <class T>
    State<T>::State(taskCfg cfg, Credentials* c, T ic) :
      ProtoState(cfg, c){
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

    

