#include "robotis.hpp"

namespace Robotis {
    RobotisPacket::RobotisPacket(){
        counter = 0;
        id = 0;
        len = 0;
        error = 0;
        checksum = 0;
        parameters = new std::vector<unsigned char>();
    }

    RobotisPacket::~RobotisPacket(){
        delete parameters;
    }


    Protocol::Protocol(std::string cfg, std::string args)
     : ACES::Protocol(cfg, args){}

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

    ACES::ProtoResult* Protocol::processUSQueue()
    {
        ACES::ProtoWord* p = NULL;
        { RTT::OS::MutexLock lock(usqGuard);
          p = usQueue.front();
          usQueue.pop_front();
        }
        ACES::Word<char*>* w = (ACES::Word<char*>*)p;
        yy_scan_bytes((const char*)(w->data), 1, this->scanner);
        //If yylex returns 1 we have matched a full packet
        //if return is 0 we have only eaten a character
        if( yylex(this->scanner) ){ 
            RobotisPacket* p  = curPacket;
            curPacket = new RobotisPacket();
            yyset_extra(curPacket, scanner) ;
            ACES::Result<RobotisPacket>* r =
                new ACES::Result<RobotisPacket>(*p,
                (ACES::Credentials*)credFromPacket(p),
                0);
            return (ACES::ProtoResult*)r;
        }
        else{
            return 0;
        }
    }

    ACES::Message* Protocol::processDSQueue()
    {
        Goal* g = getDSQelement();
        //goal carries void data
        g->data
    }

    /*
    ACES::Message* Protocol::buildMessage(
      ACES::Credentials* cred){
        std::list<unsigned char> id (1, cred->id);
        std::list<unsigned char> head (2, 0xFF);
        std::list<unsigned char> body;
        body.push_back(0x2);
        body.push_back(0x0);
        body.push_back(0x32);
        std::list<unsigned char> len (1, 0x4);
        
        std::list<unsigned char> s;
        s.splice(s.end(), id);
        s.splice(s.end(), len);
        s.splice(s.end(), body);
        
        unsigned char check = checksum(s);
        s.push_back(check);
        
        head.splice(head.end(), s);
        
        ACES::Message* m = new ACES::Message(head);
        return m;
    }
    */

    Device::Device(std::string cfg, std::string args)
     :ACES::Device(cfg)
    {
        credentials =
            (ACES::Credentials*)Credentials::makeCredentials(args);
    }

    bool Device::startHook(){
        lockout = false;
    }

    void Device::stopHook(){
    }

    Goal* Device::processDSQueue(){
        if(not lockout){
            Goal* g = getDSQelement();
            float* sp;
            RobotisPacket* p = new RobotisPacket();
            p->id = credentials->motorNum;
            p->parameters = new std::vector<unsigned char>;

            switch(g->mode){
                case(REFRESH):
                    requestPos = g->nodeID;
                    requestLen = PARAM_LEN[requestPos];
                    lockout = true;

                    p->instruct = READ;
                    p->parameters->push_back((unsigned char) g->nodeID);
                    p->parameters->push_back(
                        (unsigned char) PARAM_LEN(g->nodeID) );
                    break;
                case(SET):
                    p->instruct = WRITE;
                    p->parameters->push_back((unsigned char) g->nodeID);

                    sp = (float*)(g->data);
                    int i = reverseScale(*sp, g->nodeID);
                    //scaling function
                    //decide if two or one byte
                    //if one cast and push
                    //if two cast and push low byte then high byte
                    break;
            }
            return g;
        }
        else{
            return NULL;
        }
    }

    std::list<ACES::ProtoResult*> Device::processUSQueue(){
        ACES::ProtoResult* p = NULL;
        { RTT::OS::MutexLock lock(usqGuard);
          p = usQueue.front();
          usQueue.pop_front();
        }
        std::list<ACES::ProtoResult*> pr_list;
        //RTT::Logger::log() << "(dev) got US" << RTT::endlog();
        RobotisPacket r = ( (ACES::Result<RobotisPacket>*) p)->result;

        //requestPos, requestLen
        //map [nID]->tentry
        for(int i = 0, j = 0; i < requestLen; j = i){
            unsigned int tentry = 0;
            switch(PARAM_LEN[requestPos+i]){
                case 2:
                    unsigned char low, high;
                    //Check the order here
                    low = (*r.parameters)[i];
                    high = (*r.parameters)[i+1];
                    tentry = high;
                    tentry <<= 8;
                    tentry |= low;
                    i+=2;
                    break;
                case 1:
                    tentry = (*r.parameters)[i];
                    i++;
                    break;
                case 0:
                    assert(0);
                    break;
                default:
                    assert(0);
                    break;
            }
            float* data = new float;
            *data = (float)tentry;
            
            //This is for scaling functions, etc
            switch(requestPos+i){
                case ###:
                    break;
                case ###:
                    break;
            }

            ACES::Result<float*>* r =
                new ACES::Result<float*>(data, p->semiCred, requestPos+j);
            pr_list.push_back( (ACES::ProtoResult*)r );
        }
        lockout = false;
        return pr_list;
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

    unsigned char checksum(std::list<unsigned char> l){
        unsigned char sum = 0;
        std::list<unsigned char>::iterator it;
        for(it = l.begin(); it != l.end(); it++){
            sum += (*it);
        }
        sum = ~sum;
        return sum;
    }

    float USScale(int in, int nodeID){
    }

    int DSStreamScale(float in, int nodeID){
        int result = 0;
        switch(nodeID){
            default:        //case of no scaling function
                result = (int)in;
                break;
        }
    }
};
