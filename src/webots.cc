//Templates associated w/Webots::State    


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
