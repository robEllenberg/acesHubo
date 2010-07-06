#include "robotis.hpp"

namespace Robotis {

    Protocol::Protocol(std::string n,
        ACES::Hardware* hw, int pri, int UpdateFreq)
                   : ACES::Protocol(n, hw, pri, UpdateFreq){

        yylex_init ( &scanner ) ;
        curPacket = new RobotisPacket() ;
        yyset_extra(curPacket, scanner) ;
    }

    Protocol::~Protocol(){
        delete curPacket;
        yylex_destroy( scanner);
    }
    
    std::vector<ACES::Message*>*
    Protocol::interpreter(std::deque<char>* s){
        std::vector<ACES::Message*>* m =
           new std::vector<ACES::Message*>;
        if(s->size() == 0){
            return m;
        }
        else{
                while(! s->empty() ){
                    char c = s->front();
                    s->pop_front();
                //this->processInput(c);
                }
        }
    }

    std::deque<char>* Protocol::scanInput(){
	unsigned char c = 0;
	std::deque<char>* s = new std::deque<char>;
	while( (hwInBuffer->buffer()->Pop(c)) != 0 ){
	    s->push_back(c);
	    std::cout << "Pull " << c << std::endl;
	}
	return s;
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

ACES::Credentials* Protocol::parseHWInput(unsigned char* c){
   //RTT::Logger::log() << "parse: " <<std::hex
   //  << (int)c << std::dec << std::endl;
    yy_scan_bytes((const char*)c, 1, this->scanner);
    //If yylex returns 1 we have matched a full packet
    //if return is 0 we have only eaten a character
    if( yylex(this->scanner) ){ 
        RobotisPacket* p  = curPacket;
        curPacket = new RobotisPacket();
        unsigned char low = p->parameters->at(36);
        unsigned char high = p->parameters->at(37);
        float angle = angleScale(low, high);
        ACES::Credentials* cred =
          new ACES::Credentials( (*p).id, angle );
        delete p;
        yyset_extra(curPacket, scanner) ;
        return cred;
    }
    else{
        return 0;
    }
}

    State::State(std::string n, ACES::Credentials* c,
      ACES::Dispatcher* d, int pri, int UpdateFreq)
      : ACES::State(n, c, d, pri, UpdateFreq){
    }

    Hardware::Hardware(std::string name,
        std::ifstream *in, std::ofstream *out, int priority,
        int UpdateFreq)
            : ACES::charDevHardware(name, in,out,
                                    priority, UpdateFreq)
    {}

/* //In retrospect a bad idea
    ACES::Hardware* Hardware::operator<<(ACES::Message* m){        
        for(std::list<unsigned char>::iterator it = (*m).lineRepresentation.begin();
             it != (*m).lineRepresentation.end();
             it++){
                 //If there's a 0 in the stream we have to handle it with the special case of
                 // std::ends or the stream will ignore it.
                this->hardpoint_out->put(*it);
             }
        this->hardpoint_out->flush();
        // *this->hardpoint << std::endl;        
    }
*/

    RobotisPacket::RobotisPacket(){
        counter = 0;
        id = 0;
        len = 0;
        error = 0;
        checksum = 0;
        parameters = new std::vector<unsigned char>;
    }

    RobotisPacket::~RobotisPacket(){
        delete parameters;
    }
}
