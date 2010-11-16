#include "robotis.hpp"

namespace Robotis {
    RobotisPacket::RobotisPacket(){
        //counter = 0;
        id = 0;
        len = 0;
        error = 0;
        checksum = 0;
        parameters = new std::deque<unsigned char>();
    }

    void RobotisPacket::setID(unsigned char i){
        id = i;
    }

    void RobotisPacket::setLen(unsigned char l){
        len = l;
    }

    void RobotisPacket::setError(unsigned char err){
        error = err;
    }

    void RobotisPacket::setChecksum(unsigned char check){
        checksum = check;
    }

    void RobotisPacket::setInst(INST inst){
        instruct = inst;
    }

    unsigned char RobotisPacket::getID(){
        return id;
    }

    unsigned char RobotisPacket::getLen(){
        return len;
    }

    unsigned char RobotisPacket::getError(){
        return error;
    }

    unsigned char RobotisPacket::getChecksum(){
        return checksum;
    }

    INST RobotisPacket::getInst(){
        return instruct;
    }

    void RobotisPacket::printme(){
        RTT::Logger::log() << "ID: " << (int)id << ", Len: " << (int)len
                           << ", Error: " << (int)error 
                           << ", Check: " << (int)checksum;
        RTT::Logger::log() << ", Parameters: ";
        for(std::deque<unsigned char>::iterator it = parameters->begin();
            it != parameters->end(); it++)
        {
            RTT::Logger::log() <<  (int)(*it) << ", ";
        }
        RTT::Logger::log() << RTT::endlog();
    }

    std::ostream &operator<<(std::ostream &out, RobotisPacket &p){
        p.printme();
        return out;
    }

    //RobotisPacket::~RobotisPacket(){
    //    delete parameters;
    //}

    Hardware::Hardware(std::string cfg, std::string args)
     : ACES::Hardware<unsigned char>(cfg, args),
        io_service(),
        port(io_service, (const char*)args.c_str())
    {
        /*
        std::istringstream s1(args);
        float z;
        int id, d;
        s1 >> id >> z >> d;
        credentials = new Credentials(id, z, d);
        */
    }

    bool Hardware::txBus(ACES::Message<unsigned char>* m){
        if(m){
            ACES::Word<unsigned char>* w = NULL;
            std::vector<unsigned char> buf;
            while(m->size()){
                w = m->pop();
                buf.push_back(w->getData());
            }
            for(std::vector<unsigned char>::iterator it = buf.begin();
                it != buf.end(); it++)
            {
                //output << *it;
                //boost::array<unsigned char, 1> buf;
                //std::vector<unsigned char>buf;
                port.write_some(boost::asio::buffer((void*)(&(*it)), 1));
                RTT::Logger::log() << (int) *it  << RTT::endlog();
            }
            return true;
        }
        return false;
    }

    void Hardware::rxBus(){
        unsigned char c=0, count=1;
        ACES::Word<unsigned char> *w = NULL;
        /*
        while(count){
            //c = input.get();
            boost::array<unsigned char, 1> buf;
            //count = boost::asio::read(port, buf);
            count = port.read_some(boost::asio::buffer(buf, 1));
            c = buf[0];
            w = new ACES::Word<unsigned char>(c);
            txUpStream.write(w);
        }
        */
    }

/*
    bool Hardware::processUS(ACES::Word<unsigned char>*){
        return false;
    }

    bool Hardware::processDS(ACES::Message<unsigned char>* m){
        ACES::Word<unsigned char> *w = NULL;
        unsigned char c;
        while(m->size()){
            w = m->pop();
            c = w->getData();
            output << c;
        }
        return true;
    }
*/
    Protocol::Protocol(std::string cfg, std::string args)
     : ACES::Protocol<unsigned char, RobotisPacket>(cfg, args){}

    bool Protocol::startHook(){
        yylex_init ( &scanner ) ;
        curPacket = new RobotisPacket() ;
        yyset_extra(curPacket, scanner) ;
        return true;
    }

    void Protocol::stopHook(){
        delete curPacket;
        yylex_destroy(scanner);
    }

    ACES::Word<RobotisPacket>*
      Protocol::processUS(ACES::Word<unsigned char>* usIn)
    {
        ACES::Word<RobotisPacket>* pw = NULL;
        unsigned char c = 0;
        c = usIn->getData();
        yy_scan_bytes( (const char*)(&c), 1, this->scanner);
        //If yylex returns 1 we have matched a full packet
        //if return is 0 we have only eaten a character
        if( yylex(this->scanner) ){ 
            RobotisPacket* p  = curPacket;
            curPacket = new RobotisPacket();
            yyset_extra(curPacket, scanner) ;

            pw = new ACES::Word<RobotisPacket>(*p, 0, 0, 0,
                (ACES::Credentials*)credFromPacket(p));
            return pw;
        }
        return NULL;
    }

    ACES::Message<unsigned char>*
        Protocol::processDS(ACES::Word<RobotisPacket>* w)
    {
        RobotisPacket p = w->getData();
        //p.printme();
        /*
        switch(p->instruct){
            case PING:
                break;
            case READ:
                break;
            case WRITE:
                break;
            case REG_WRITE:
                break;
            case ACTION:
                break;
            case RESET:
                break;
            case SYNC_WRITE:
                break;
            default:
                return NULL;
        }
        */
        ACES::Message<unsigned char>* m = messageFromPacket(&p);
        //m->printme();
        return m;

        //unsigned char* c = new unsigned char;
        //e = new ACES::Goal(g->nodeID, g->mode, (void*)
        //m->goalList.push_back(e));
    }

    Device::Device(std::string cfg, std::string args)
     :ACES::Device<float, RobotisPacket>(cfg)
    {
        credentials =
            (ACES::Credentials*)Credentials::makeCredentials(args);
    }

    //bool Device::startHook(){
        //lockout = false;
        //return true;
    //}

    //void Device::stopHook(){
    //}

    ACES::Word<RobotisPacket>* Device::processDS(ACES::Word<float>* w){
        if(w){
            RobotisPacket p;
            p.setID(credentials->getDevID());
            p.parameters = new std::deque<unsigned char>;
            float sp = 0.;
            switch( w->getMode() ){
                case(ACES::REFRESH):
                    //requestion Position in the motor control table
                    requestPos = w->getNodeID();
                    //length of the request - # bytes
                    requestLen = PARAM_LEN[requestPos];
                    //lockout = true;

                    p.setInst(READ);
                    p.parameters->push_back((unsigned char) requestPos);
                    p.parameters->push_back((unsigned char) requestLen);
                    break;

                case(ACES::SET):
                    p.setInst(WRITE);
                    p.parameters->push_back((unsigned char) w->getNodeID());
                    
                    sp = w->getData();     //Grab set-point from goal
                    //Scale the set-point
                    unsigned short ssp = DSScale(sp, w->getNodeID()); 
                    RTT::Logger::log() << "setpoint " << ssp << RTT::endlog();
                    //Byte-chop the scaled point and add it to the param list
                    appendParams(p.parameters, ssp,
                                 PARAM_LEN[ w->getNodeID() ] );
                    break;
            }
            ACES::Word<RobotisPacket> *pw = new ACES::Word<RobotisPacket>(p);
            return pw;
        }
        return NULL;
    }

    ACES::Word<float>* Device::processUS(ACES::Word<RobotisPacket>* w){
        RobotisPacket* p = &(w->getData());

        int requestPos = w->getNodeID();
        
        //map [nID]->tentry
        for(int i = 0; i < requestLen; ){
            unsigned short tentry = 0;
            unsigned char low=0, high=0;
            switch(PARAM_LEN[requestPos+i]){
                case 2:
                    //Check the order here
                    low = (*(p->parameters))[i];
                    high = (*(p->parameters))[i+1];
                    tentry = high;
                    tentry <<= 8;
                    tentry |= low;
                    i+=2;
                    break;
                case 1:
                    tentry = (*(p->parameters))[i];
                    i++;
                    break;
                case 0:
                    assert(0);
                    break;
                default:
                    assert(0);
                    break;
            }
            //The scaling function
            float data = USScale(tentry, requestPos+i);

            ACES::Word<float> *res = new ACES::Word<float>(data, requestPos+i,
                                                     credentials->getDevID(),
                                                     ACES::REFRESH,
                                                     credentials);
            txUpStream.write(res);
        }
        //lockout = false;
        return NULL;
    }

    Credentials::Credentials(int motNum, float z, float dir)
     :ACES::Credentials(motNum)
    {
        motorNum = motNum;
        zero = z;
        direction = dir;
    }

    Credentials* Credentials::makeCredentials(std::string args){
        //Format is "IDnum zero direction"
        std::istringstream s1(args);
        float z, d;
        int num;
        s1 >> num >> z >> d;
        return new Credentials(num, z, d);
    }

    void Credentials::printme(){
        ACES::Credentials::printme();
        RTT::Logger::log() << "(Robotis) Credentials: Motor Number="
                           << motorNum
                           << " Zero=" << zero << RTT::endlog();
    }

    bool Credentials::operator==(ACES::Credentials& other){
        if(not ACES::Credentials::operator==(other) ){
            return false;
        }
        if(dynamic_cast<Credentials*>(&other) ){
            Credentials* c = (Credentials*)&other;
            if(c->motorNum == motorNum){
                return true;
            }
            else{
                return false;
            }
        }
        return false;
    }

    Credentials* credFromPacket(RobotisPacket* p){
        //We need to generate a packet which will pass through the
        //comparison operator on the motor of interest, the zero and direction
        //information don't matter
        return new Credentials(p->getID(), 0.0, 0.0);
    }

    ACES::Message<unsigned char>* messageFromPacket(RobotisPacket* p){
        ACES::Message<unsigned char> *m = new ACES::Message<unsigned char>();
        p->setLen( p->parameters->size() + 2 );
        p->setChecksum( checksum(p) );

        //Build version for sending
        //Header
        m->push(new ACES::Word<unsigned char>(0xFF));
        m->push(new ACES::Word<unsigned char>(0xFF));
        m->push(new ACES::Word<unsigned char>(p->getID()));
        m->push(new ACES::Word<unsigned char>(p->getLen()));
        m->push(new ACES::Word<unsigned char>((unsigned char)p->getInst()));
        //Body
        for(std::deque<unsigned char>::iterator it = p->parameters->begin();
            it != p->parameters->end(); it++)
        {
            m->push(new ACES::Word<unsigned char>(*it));
        }
        //Checksum
        m->push(new ACES::Word<unsigned char>(p->getChecksum()) );

        return m;
    }

    unsigned char checksum(RobotisPacket* p){
        //Checksum must be generated using entire packet less the leading
        // 0xFF bytes (x2), and the checksum (obviously)
        unsigned char sum = 0;
        sum += p->getID();
        sum += p->getLen();
        sum += (unsigned char)p->getInst();
        std::deque<unsigned char>::iterator it;
        for(it = p->parameters->begin(); it != p->parameters->end(); it++){
            sum += (*it);
        }
        sum = ~sum;
        return sum;
    }

    float USScale(unsigned short in, int nodeID){
        float result = 0.;
        switch(nodeID){
            case GOAL_POSITION:
                result = (in*300./1024.);
                result = limit<float>(result, 0., 300.);
                break;
            default:        //case of no scaling function
                result = (float)in;
                break;
        }
        return result;
    }

    unsigned short DSScale(float in, int nodeID){
        unsigned short result = 0;
        switch(nodeID){
            case GOAL_POSITION:
                //TODO - needs proper rounding
                result = (unsigned short)(in*1023./300.);
                result = limit<unsigned short>(result, 0, 1023);
                break;
            default:        //case of no scaling function
                result = (unsigned short)in;
                break;
        }
        return result;
    }

    template <class T>
    T limit(float c, T low, T high){
        if(c < low){
            return low;
        }
        if(c > high){
            return high;
        }
        return (T)c;
    }

    bool appendParams( std::deque<unsigned char>* params,
                       unsigned short data, int size )
    {
        //we are assuming that shorts are 16 bit
        unsigned char low = 0, high = 0;
        int temp = 0;
        //Figure out how to cast these properly
        low = (unsigned short)(data & 0x00FF);
        temp = (unsigned short)(data & 0xFF00);
        high = temp >> 8;
        switch(size){
            case(2):
                params->push_back(low);
                params->push_back(high);
                break;
            case(1):
                params->push_back(low);
                break;
            default:
                assert(0);
                break;
        }
        return true;
    }
};
