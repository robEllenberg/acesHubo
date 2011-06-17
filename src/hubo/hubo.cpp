/* AUTORIGHTS
    ACES: Abstracted Controls Engineering Software
    Copyright (C) 2010 Robert Sherbert
    bob.sherbert@gmail.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    If you are interested in licensing this software for commercial purposes
    please contact the author.
*/

#include "hubo.hpp"

namespace HuboVia{
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

namespace Hubo{
    #define TESTMODE 1
    CANHardware::CANHardware(std::string cfg, std::string args)
      : ACES::Hardware<canmsg_t*>(cfg, args)
    {
        //args contains the CAN device we're interested in e.g. "/dev/can0"
        //the baud rate to operate at 
        // args = "/dev/someFD <(int)baudrate>"
        std::istringstream s(args);
        s >> fd >> rate;

        this->addAttribute("rate", rate);
        this->addAttribute("fileNode", fd);

        //Add a loopback connection that the device can use to add to its own
        //queue
        this->ports()->addPort("TxDSLoopback", txDSLoop).doc(
                               "Downstream (to Bus) Self Connection");
        RTT::base::PortInterface *tx = NULL, *rx=NULL;
        RTT::ConnPolicy policy = RTT::ConnPolicy::buffer(200);
        tx = (RTT::base::PortInterface*)this->ports()->getPort("TxDSLoopback");
        rx = (RTT::base::PortInterface*)this->ports()->getPort("RxDS");
        bool success = tx->connectTo(rx, policy);
        assert(success);
        
        this->addOperation("genPacket", &CANHardware::genPacket, this,
                               RTT::OwnThread).doc("Transmit a packet on the"
                               " CAN line")
                               .arg("ID", "Message ID")
                               .arg("len", "Message Length")
                               .arg("b0123", "Lower Order Bytes - Hex String")
                               .arg("b4567", "Upper Order Bytes - Hex String");
    }

    bool CANHardware::startHook(){
        channel = open(fd.c_str(), O_RDWR | O_NONBLOCK);

        #if TESTMODE == 1
            // Perform no special commands    
        #else //Normal operation action
            //Begin - snippit from can4linux examples baud.c
            Config_par_t  CANcfg;
            Command_par_t cmd;

            cmd.cmd = CMD_STOP;
            ioctl(channel, CAN_IOCTL_COMMAND, &cmd);

            CANcfg.target = CONF_TIMING; 
            CANcfg.val1   = rate;
            ioctl(channel, CAN_IOCTL_CONFIG, &CANcfg);

            cmd.cmd = CMD_START;
            ioctl(channel, CAN_IOCTL_COMMAND, &cmd);
            //End snippit
        #endif
        return true;
    }
    
    void CANHardware::stopHook(){
        close(channel);
    }

    bool CANHardware::genPacket(int ID, int len, 
                                std::string lower, std::string upper)
    {
        ACES::Message<canmsg_t*> *m = new ACES::Message<canmsg_t*>();
        unsigned int b0123 = 0, b4567 = 0;
        std::istringstream u(upper), l(lower);
        l >> std::setbase(16) >> b0123;
        u >> std::setbase(16) >> b4567;

        canmsg_t* msg = new canmsg_t;
        msg->id = ID;
        msg->cob = 0;
        msg->flags = 0;
        msg->length = len;
        switch(len){
            case 8:
                msg->data[7] = ((b4567 & 0x000000FFu) >> (8*0));
            case 7:
                msg->data[6] = ((b4567 & 0x0000FF00u) >> (8*1));
            case 6:
                msg->data[5] = ((b4567 & 0x00FF0000u) >> (8*2));
            case 5:
                msg->data[4] = ((b4567 & 0xFF000000u) >> (8*3));
            case 4:
                msg->data[3] = ((b0123 & 0x000000FFu) >> (8*0));
            case 3:
                msg->data[2] = ((b0123 & 0x0000FF00u) >> (8*1));
            case 2:
                msg->data[1] = ((b0123 & 0x00FF0000u) >> (8*2));
            case 1:
                msg->data[0] = ((b0123 & 0xFF000000u) >> (8*3));
            case 0:
                break;
            default:
                assert(0);
        }
        m->push( new ACES::Word<canmsg_t*>(msg) );

        txDSLoop.write(m);
        return true;
    }

    bool CANHardware::txBus(ACES::Message<canmsg_t*>* m){
        while( m->size() ){
            ACES::Word<canmsg_t*>* w = m->pop();
            canmsg_t* msg = w->getData();
            #if TESTMODE == 1
                void* p = &(msg->data);
                int r = write(channel, p, msg->length);
            #else //Normal operation
                int r = write(channel, msg, 1);
            #endif 
            if(r == -1){
                RTT::Logger::log() << RTT::Logger::Info
                << "CAN Transmission Write Error"
                << RTT::endlog();
            } else if(r == 0){
                RTT::Logger::log() << RTT::Logger::Info
                << "CAN Transmission Timeout"
                << RTT::endlog();
            }
            delete msg;
            //TODO - Delete the word
        }
        delete m;
        return true;
    }

    void CANHardware::rxBus(int size){
    }

    Protocol::Protocol(std::string cfg, std::string args)
      :ACES:Protocol<canmsg_t*, canMsg>(cfg, args)
    {}

    ACES::Message<canmsg_t*>* Protocol::processDS(ACES::Word<canMsg>* h){
        ACES::Message<canmsg_t*> *m = NULL;
        if(h){
            canmsg_t* msg = h->getData()->toLineType();
            m = new ACES::Message<canmsg_t*>;
            m->push( new ACES::Word<canmsg_t*>(msg) );
        }
        return m;
    }
}
