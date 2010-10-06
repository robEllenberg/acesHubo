    template <class T>
    State<T>::State(std::string cfg, int nID) :
      ProtoState(cfg, nID),
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
        Goal* g = new Goal(nodeID, SET, p);
        set_stack->Push(g);
        //txDownStream(g);
    }

    template <class T>
    void State<T>::assign(ProtoResult* meas, void* me){
        State<T>* th = (State<T>*)me;
        //t->value = *((T*)meas);
        T* newVal = ((Result<T*>*)meas)->result;
        RTT::Logger::log() << RTT::Logger::Debug << "(state) Value: " << *newVal;
        th->value.set(  *newVal  );
    }

