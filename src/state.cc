    template <class T>
    State<T>::State(std::string cfg, std::string args) :
      ProtoState(cfg, args){
        value = 0;
    }
    
    template <class T>
    void State<T>::printme(){
        RTT::Logger::log() << "State: " << this->name
        << ", (F:" << this->frequency << ", P:"
        << this->priority << "),"
        << "(Val = " << value << ")" << std::endl;

        this->credentials->printme();
    }

