#include "robotis.hpp"

namespace Robotis {
    Protocol::Protocol(std::string cfg, std::string args){
     :ACES::Protocol(cfg, args){}

    bool Protocol::startHook(){
        yylex_init ( &scanner ) ;
        curPacket = new RobotisPacket() ;
        yyset_extra(curPacket, scanner) ;
        return true;
    }

    Protocol::stopHook(){
        delete curPacket;
        yylex_destroy(scanner);
    }
    
    Credentials credFromPacket(RobotisPacket* p){
        //We need to generate a packet which will pass through the
        //comparison operator on the motor of interest, the zero and direction
        //information don't matter
        return Credentials(p->id, 0.0, 0.0);
    }

    ProtoResult* Protocol::processUSQueue()
    {
        ProtoWord* p = NULL;
        { RTT::OS::MutexLock lock(usqGuard);
          p = usQueue.front();
          usQueue.pop_front();
        }
        Word<char*>* w = (Word<char*>*)p;
        yy_scan_bytes((const char*)(w->data), 1, this->scanner);
        //If yylex returns 1 we have matched a full packet
        //if return is 0 we have only eaten a character
        if( yylex(this->scanner) ){ 
            RobotisPacket* p  = curPacket;
            curPacket = new RobotisPacket();
            yyset_extra(curPacket, scanner) ;
            Result<RobotisPacket>* r = new Result<RobotisPacket>(*p, credFromPacket(p), 0);
            return (ProtoResult*)r;
        }
        else{
            return 0;
        }
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

    float angleScale(unsigned char low, unsigned char high){
        unsigned int base = high;
        base <<= 8;
        base |= low;
        //Scale from range (0,1024) to (0,300)
        float angle = 300./1024.* base;  
        return angle;
    }
        
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

    Device::Device(std::string cfg, std::string args)
     :ACES::Device(cfg)
    {
        credentials = (ACES::Credentials*) makeCredentials(std::string args);
    }

    bool Device::startHook(){
    }

    void Device::stopHook(){
    }

    std::list<ProtoResult*> Device::processUSQueue(){
        ProtoResult* p = NULL;
        { RTT::OS::MutexLock lock(usqGuard);
          p = usQueue.front();
          usQueue.pop_front();
        }
        std::list<ProtoResult*> pr_list();
        //RTT::Logger::log() << "(dev) got US" << RTT::endlog();
        RobotisPacket p = ( (Result<RobotisPacket>*) p)->result;

        requestPos, requestLen
        for(int i = 0; i < requestLen; ){
            switch(PARAM_LEN[requestPos+i]){
                case 2:
                    i+=2;
                    break;
                case 1:
                    i++;
                    break;
                case 0:
                    assert(0);
                    break;
                default:
                    assert(0);
                    break;
            }

        }
            
        for(std::vector<unsigned char>::iterator it = p.parameters.begin();
            it != p.parameters.end(); it++)
        {
            
        }

        Result<Robotis*>* r = new Result<void*>(g->data, g->cred, p->nodeID);
        return pr_list;

            /*
            unsigned char low = p->parameters->at(36);
            unsigned char high = p->parameters->at(37);
            float angle = angleScale(low, high);
            ACES::Credentials* cred =
              new ACES::Credentials( (*p).id, angle );
            delete p;
            */
    }

    Credentials::Credentials(int motNum, float zero, float dir)
     :ACES::Credentials((int)JOINT)
    {
        motorNum = motNum;
        angle = ang;
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
                           << " Angle=" << angle << RTT::endlog();
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
}
