//Templates associated w/Webots::State    
    template <class T>
    ACES::Credentials* State<T>::parseDispArgs(std::string type,
                                         std::string args)
    {
        ACES::Credentials *c = 0;
        if(type == "float"){
            std::istringstream s1(args);
            std::string id;
            float zero, rot;
            s1 >> id >> zero >> rot;
            c = (ACES::Credentials*)new Credentials(id, zero, rot);
        }
        return c;
    }

    template <class T>
    State<T>::State(std::string n,
      ACES::Credentials* c,
      int pri, int UpdateFreq)
      : ACES::State<T>(n, c, pri, UpdateFreq)
    {}

    template <class T>
    State<T>::State(std::string pname, std::string cname,
      int pri, int UpdateFreq,
      float z, float rot)
        : ACES::State<T>(pname,
                          new Credentials(cname,z,rot),
                          pri, UpdateFreq)
    {}

    template <class T>
    State<T>::State(ACES::taskCfg cfg, ACES::Credentials* c,
                    T ic)
     :ACES::State<T>(cfg, c, ic)
    {}
