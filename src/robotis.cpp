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

    RobotisPacket::~RobotisPacket(){
        delete parameters;
    }


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
            RobotisPacket p;
            p.id = credentials->getDevID();
            p.parameters = new std::deque<unsigned char>;
            float sp = 0.;
        if(w){
            switch( w->getMode() ){
                case(ACES::REFRESH):
                    //requestion Position in the motor control table
                    requestPos = w->getNodeID();
                    //length of the request - # bytes
                    requestLen = PARAM_LEN[requestPos];
                    //lockout = true;

                    p.instruct = READ;
                    p.parameters->push_back((unsigned char) w->getNodeID());
                    p.parameters->push_back(
                        (unsigned char) PARAM_LEN[w->getNodeID()] );
                    break;

                case(ACES::SET):
                    p.instruct = WRITE;
                    p.parameters->push_back((unsigned char) w->getNodeID());
                    
                    sp = w->getData();     //Grab set-point from goal
                    //Scale the set-point
                    unsigned short ssp = DSScale(sp, w->getNodeID()); 
                    //Byte-chop the scaled point and add it to the param list
                    appendParams(p.parameters, ssp,
                                 PARAM_LEN[w->getNodeID()] );
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
     :ACES::Credentials((int)JOINT)
    {
        motorNum = motNum;
        zero = z;
        direction = dir;
    }

    static Credentials* makeCredentials(std::string args){
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
        return new Credentials(p->id, 0.0, 0.0);
    }

    ACES::Message<unsigned char>* messageFromPacket(RobotisPacket* p){
        ACES::Message<unsigned char> *m = new ACES::Message<unsigned char>();
        p->len = p->parameters->size() + 2;
        p->checksum = checksum(p);

        //Build version for sending
        //Header
        m->push(new ACES::Word<unsigned char>(0xFF));
        m->push(new ACES::Word<unsigned char>(0xFF));
        m->push(new ACES::Word<unsigned char>(p->id));
        m->push(new ACES::Word<unsigned char>(p->len));
        m->push(new ACES::Word<unsigned char>((unsigned char)p->instruct));
        //Body
        for(std::deque<unsigned char>::iterator it = p->parameters->begin();
            it != p->parameters->end(); it++)
        {
            m->push(new ACES::Word<unsigned char>(*it));
        }
        //Checksum
        m->push(new ACES::Word<unsigned char>(p->checksum));

        return m;
    }

    unsigned char checksum(RobotisPacket* p){
        //Checksum must be generated using entire packet less the leading
        // 0xFF bytes (x2), and the checksum (obviously)
        unsigned char sum = 0;
        sum += p->id;
        sum += p->len;
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
                result = USlimit(result, 0., 300.);
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
                result = (unsigned short)(in*1024./300.);
            default:        //case of no scaling function
                result = (unsigned short)in;
                break;
        }
        return result;
    }

    float USlimit(float c, float low, float high){
        if(c < low){
            return low;
        }
        if(c > high){
            return high;
        }
        return c;
    }

    bool appendParams( std::deque<unsigned char>* params,
                       unsigned short data, int size )
    {
        //we are assuming that shorts are 16 bit
        unsigned char low = 0, high = 0;
        //Figure out how to cast these properly
        low = data & (unsigned short)0x00FF;
        high = data & (unsigned short)0xFF00;
        switch(size){
            case(2):
                params->push_back(low);
                params->push_back(high);
            case(1):
                params->push_back(low);
            default:
                assert(0);
        }
        return true;
    }
};
