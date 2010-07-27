    template <class T>
    State<T>::State(std::string cfg, std::string args) :
      ProtoState(cfg, args),
      goMethod("go", &State<T>::go, this),
      value("value")
    {
        //value = 0;
        bool one = this->methods()->addMethod(goMethod, "go",
                                   "SP",
                                   "Set Point");
        this->attributes()->addAttribute(&value);
        asgnfunct = assign;
    }

/*
    template <class T>
    State<int>::State(std::string cfg, std::string args){
        dtype = INT;
    }
*/    
    template <class T>
    void State<T>::printme(){
        RTT::Logger::log() << "State: " << name
        << ", (F:" << freq << ", P:"
        << priority << "),"
        << "(Val = " << value.get() << ")" << std::endl;
    }

    template <class T>
    void State<T>::go(T sp){
        T* p = new T(sp);
        Goal* g = new Goal(propID, SET, p);
        announceGoal(g);
    }

    template <class T>
    void State<T>::assign(void* meas, void* me){
        State<T>* t = (State<T>*)me;
        //t->value = *((T*)meas);
        t->value.set(  *((T*)meas)  );
    }

