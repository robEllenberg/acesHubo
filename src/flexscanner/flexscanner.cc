namespace FlexScanner{
    Hardware::Hardware(std::string cfg, std::string args)
     : ACES::Hardware<unsigned char>(cfg, args),
        io_service(),
        port(io_service, (const char*)args.c_str()),
        rxReader(&port),
        rxActivity(priority, &rxReader)
    {
        RTT::ConnPolicy policy = RTT::ConnPolicy::buffer(200);
        rxReader.out.connectTo(&rxBuf, policy);
    }
    
    bool Hardware::startHook(){
        rxActivity.start();
        return true;
    }

    void Hardware::stopHook(){
        rxActivity.stop();
    }

    void Hardware::rxBus(int size){
        unsigned char c;
        while(rxBuf.read(c) == RTT::NewData){
            RTT::Logger::log() << "Got Something! - " << (int)c
                               << RTT::endlog();
            txUpStream.write(new ACES::Word<unsigned char>(c));
        }
    }

    Reader::Reader(boost::asio::serial_port* p)
     : RTT::base::RunnableInterface()
    {
        port = p;
    }

    bool Reader::initialize(){
        return true;
    }

    void Reader::step(){
    }

    void Reader::loop(){
        while(true){
            boost::asio::read(*port, boost::asio::buffer((void*)&rxBuf, 1));
            out.write(rxBuf);
        }
    }

    void Reader::finalize(){
    }

    template <class USPacket, class FlexScanner>
    Protocol<USPacket, FlexScanner>::Protocol(std::string cfg, std::string args)
     : ACES::Protocol<unsigned char, USPacket>(cfg, args)
    {
        scanner = NULL;
    }

    template <class USPacket, class FlexScanner>
    bool Protocol<USPacket, FlexScanner>::startHook(){
        scanner = new FlexScanner();
        return true;
    }

    template <class USPacket, class FlexScanner>
    void Protocol<USPacket, FlexScanner>::stopHook(){
        delete scanner;
        scanner = NULL;
    }

    template <class USPacket, class FlexScanner>
    ACES::Word<USPacket>*
      Protocol<USPacket, FlexScanner>::processUS(ACES::Word<unsigned char>* usIn)
    {
        ACES::Word<USPacket>* pw = NULL;
        unsigned char c = 0;
        c = usIn->getData();
        //RTT::Logger::log() << "Got Something! - " << (int)c << RTT::endlog();

        std::string temp(1, c);
        istringstream *in = new istringstream(temp);
        
        //If yylex returns 1 we have matched a full packet
        //if return is 0 we have only eaten a character
        if( scanner->yylex((istream*)in) ){ 
            USPacket* p = scanner->releasePacket();

            ACES::Word<USPacket>* word= new ACES::Word<USPacket>(*p, 0, 0, 0,
                (ACES::Credentials*)credFromPacket(p));
            p->printme();
            delete in;
            delete p;
            return word;
        }
        return NULL;
    }
}
