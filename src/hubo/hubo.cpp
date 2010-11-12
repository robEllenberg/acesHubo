#include "hubo.hpp"

namespace Hubo{
    HuboVia::HuboVia(double pos){
        rad = pos;
    }

    HuboVia::HuboVia(){
        rad = 0;
    }

    double HuboVia::getRad(){
        return rad;
    }

    double HuboVia::getDeg(){
        return 180.*rad/ (M_PI);
    }

    Credentials::Credentials(int id, double z, int dir)
     : ACES::Credentials(id){
        zero = z;
        direction = dir;
    }

    double Credentials::getZero(){
        return zero;
    }

    int Credentials::getDirection(){
        return direction;
    }

    void Credentials::printme(){
        RTT::Logger::log() << "(Hubo) Credentials: devID=" << devID
                           << " zero=" << zero << " dir=" << direction
                           << RTT::endlog();
    }

    Hardware::Hardware(std::string cfg, std::string args)
     :ACES::Hardware<unsigned char*>(cfg, args),
      io_service(),
      resolver(io_service),
      query(udp::v4(), "192.168.0.193", "11000"),
      socket(io_service)
    {
        //try{
        receiver_endpoint = *resolver.resolve(query);
        socket.open(udp::v4());
        //}
    }

    bool Hardware::startHook(){
        //boost::asio::io_service io_service;
        //udp::resolver resolver(io_service);
        //udp::resolver::query query(udp::v4(), "192.168.0.193", 11000);
        //udp::endpoint receiver_endpoint = *resolver.resolve(query);
        //udp::socket socket(io_service);
        //socket.open(udp::v4());

        return true;
    }

    void Hardware::stopHook(){
    }

    //bool Hardware::processDS(Message<unsigned char*>*);
    bool Hardware::txBus(ACES::Message<unsigned char*>* m){
        RTT::Logger::log() << "Message: ";
        unsigned char *buf = m->pop()->getData();
        for(int i = 0; i < 8; i++){
            RTT::Logger::log() << std::hex << (int) buf[i] << ", ";
            send_buf[i] = buf[i];
        }
        RTT::Logger::log() << RTT::endlog();
        socket.send_to(boost::asio::buffer(send_buf), receiver_endpoint);
        return true;
    }

    Protocol::Protocol(std::string cfg, std::string args)
     :ACES::Protocol<unsigned char*, HuboVia>(cfg, args)
    {}

    ACES::Message<unsigned char*>* Protocol::processDS(ACES::Word<HuboVia>* h){
        ACES::Message<unsigned char*> *m = NULL;
        if(h){
            unsigned char *buf = new unsigned char[8];
            getHuboTx( (unsigned char)h->getCred()->getDevID(),
                       h->getData().getDeg(), buf);
            m = new ACES::Message<unsigned char*>;
            m->push(new ACES::Word<unsigned char*>(buf));
        }
        return m;
    }

    Device::Device(std::string cfg, std::string args)
     : ACES::Device<float, HuboVia>(cfg)
    {
        std::istringstream s1(args);
        float z;
        int id, d;
        s1 >> id >> z >> d;
        credentials = new Credentials(id, z, d);
    }

    ACES::Word<HuboVia>* Device::processDS(ACES::Word<float>* s){
        float pos = s->getData();       //User given set point

        //Calculate the absolute command based on the configured zero and
        //direction.
        Credentials* c = (Credentials*)credentials;
        double goal = c->getDirection()*pos + c->getZero(); 
        HuboVia v(goal);

        //Copy word info from state
        ACES::Word<HuboVia>* w = new ACES::Word<HuboVia>(v, s->getNodeID(),
                                                         s->getDevID(),
                                                         s->getMode(), NULL);
        w->setCred(credentials);    
        return w;
    }
};
