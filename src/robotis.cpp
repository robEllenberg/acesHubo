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

    bool RobotisQueue::addPacket(RobotisPacket p){
        PARAM_TABLE parameter = (PARAM_TABLE)(p.parameters->operator[](0) );
        int motorID = p.getID();
        RTT::os::MutexLock lock(qGuard);
        queue[parameter][motorID] = p;
        return true;
    }

    std::vector<ACES::Message<unsigned char>*>
      RobotisQueue::renderAllMessages()
    {
        std::vector<ACES::Message<unsigned char>*> v;
        RTT::os::MutexLock lock(qGuard);
        for(std::map<PARAM_TABLE, std::map<unsigned char, RobotisPacket> >
            ::iterator it = queue.begin(); it != queue.end(); it++)
        {
            v.push_back( renderMessageFromPackets((*it).second) );
            queue.erase(it);
        }
        return v;
    }

/*
    ACES::Message<unsigned char>* RobotisQueue::renderMessageFromQueue(
                                                PARAM_TABLE param)
    {
        ACES::Message<unsigned char>* m = NULL;
        if(queue.find(param) != queue.end()){
            m = renderMessageFromPackets(queue[param]);
            return m;
        }
        return NULL;
    }
*/
    ACES::Message<unsigned char>* RobotisQueue::renderMessageFromPackets(
                                    std::map<unsigned char, RobotisPacket>& p)
    {
        RobotisPacket rp;
        rp.setID(0xFE);
        rp.setInst(SYNC_WRITE);
        rp.parameters = new std::deque<unsigned char>;
        
        //Assure that the map is not empty
        unsigned char requestPos = 0, requestLen = 0; 
        if(p.size()){
            (*(p.begin())).second.printme();
            requestPos = ( *(p.begin()) ).second.parameters->operator[](0);
            requestLen = ( *(p.begin()) ).second.parameters->size() - 1;
            //RTT::Logger::log() << "requestLen is " << (int)requestLen <<
            //RTT::endlog();
            rp.parameters->push_back(requestPos);
            rp.parameters->push_back(requestLen);
        }
        else{
            return NULL;
        }
        for(std::map<unsigned char, RobotisPacket>::iterator it = p.begin();
            it != p.end(); it++)
        {
            rp.parameters->push_back( (*it).first );
            for(int i=0; i < requestLen; i++){
               rp.parameters->push_back( (*it).second.parameters
                                         ->operator[](1+i) );
            }
        }

        return messageFromPacket(&rp);
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

    Hardware::Hardware(std::string cfg, std::string args)
     : ACES::Hardware<unsigned char>(cfg, args),
        io_service(),
        port(io_service, (const char*)args.c_str()),
        rxReader(&port),
        rxActivity(priority, &rxReader)
    {
        RTT::ConnPolicy policy = RTT::ConnPolicy::buffer(200);
        rxReader.out.connectTo(&rxBuf, policy);
        /*
        std::istringstream s1(args);
        float z;
        int id, d;
        s1 >> id >> z >> d;
        credentials = new Credentials(id, z, d);
        */
    }
    
    bool Hardware::startHook(){
        rxActivity.start();
        return true;
    }

    void Hardware::stopHook(){
        rxActivity.stop();
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
                port.write_some(boost::asio::buffer((void*)(&(*it)), 1));
                RTT::Logger::log() << (int) *it  << RTT::endlog();
            }
            return true;
        }
        return false;
    }

    void Hardware::rxBus(int size){
        //io_service.poll();
        //io_service.run();
        //std::iostream* i = (std::iostream*)port.native();
        //i->read(&rxBuf, 1);
        //RTT::Logger::log() << "Handler " << (int) rxBuf << RTT::endlog();
        unsigned char c;
        while(rxBuf.read(c) == RTT::NewData){
            //rxBuf = new boost::asio::buffer((void*)(new unsigned char c[size]), size) b1;
            RTT::Logger::log() << "Got Something! - " << (int)c << RTT::endlog();
            txUpStream.write(new ACES::Word<unsigned char>(c));

            //port.async_read_some(boost::asio::buffer((void*)&rxBuf, 1),
            //                     Hardware::rxHandle);
            //boost::asio::async_read(port, boost::asio::buffer((void*)&rxBuf,
            //1), Hardware::rxHandle);
            //port.read_some(boost::asio::buffer( (void*)&rxBuf, 1) );
            //RTT::Logger::log() << "Handler " << (int) rxBuf << RTT::endlog();
            /*port.async_read_some(boost::asio::buffer((void*)&rxBuf, 1),
                               boost::bind(&Hardware::rxHandle, this,
                               boost::asio::placeholders::error,
                               boost::asio::placeholders::bytes_transferred
                               ));
            */
        }
    }

/*
    void Hardware::rxHandle(//Hardware* This,
                            const boost::system::error_code& error,
                            int numBytes)
    {
        RTT::Logger::log() << "Handler" << RTT::endlog();
        for(int i=0; i< numBytes; i++){
            ACES::Word<unsigned char> *w = NULL;
            w = new ACES::Word<unsigned char>(This->rxBuf);
            RTT::Logger::log() << "Got " << (int) This->rxBuf <<
            RTT::endlog();
            This->txUpStream.write(w);
        }
        This->rxBus(1);
    }
*/     

    Protocol::Protocol(std::string cfg, std::string args)
     : ACES::Protocol<unsigned char, RobotisPacket>(cfg, args),
       triggerDS(true)
    {
        this->addAttribute("triggerDS", triggerDS);
    }

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
        //RTT::Logger::log() << "Got Something! - " << (int)c << RTT::endlog();
        yy_scan_bytes( (const char*)(&c), 1, this->scanner);
        //If yylex returns 1 we have matched a full packet
        //if return is 0 we have only eaten a character
        if( yylex(this->scanner) ){ 
            RobotisPacket* p  = curPacket;
            curPacket = new RobotisPacket();
            yyset_extra(curPacket, scanner) ;

            pw = new ACES::Word<RobotisPacket>(*p, 0, 0, 0,
                (ACES::Credentials*)credFromPacket(p));
            p->printme();
            return pw;
        }
        return NULL;
    }

    ACES::Message<unsigned char>*
        Protocol::processDS(ACES::Word<RobotisPacket>* w)
    {
        RobotisPacket p = w->getData();
        ACES::Message<unsigned char>* m = NULL;
        //p.printme();
        switch(p.getInst()){
            case PING:
                //Intentional fallthrough
            case READ:
                m = messageFromPacket(&p);
                break;
            case WRITE:
                //m = messageFromPacket(&p);
                queue.addPacket(p);
                break;
            case REG_WRITE:
                //Intentional Fallthrough
            case ACTION:
            case RESET:
            case SYNC_WRITE:
            default:
                return NULL;
        }
        //m->printme();
        return m;

        //unsigned char* c = new unsigned char;
        //e = new ACES::Goal(g->nodeID, g->mode, (void*)
        //m->goalList.push_back(e));
    }

    void Protocol::txDSPending(){
        if(triggerDS){
            std::vector<ACES::Message<unsigned char>*> v =
                queue.renderAllMessages();

            for(std::vector<ACES::Message<unsigned char>*>::iterator it =
                v.begin(); it != v.end(); it++)
            {
                txDownStream.write(*it);
            }
        }
    }

    Device::Device(std::string cfg, std::string args)
     :ACES::Device<float, RobotisPacket>(cfg),
     requestPos(-1), requestLen(-1)
    {
        credentials =
            (ACES::Credentials*)Credentials::makeCredentials(args);
        this->addOperation("getTable", &Device::getTable, this,
                        RTT::OwnThread).doc("Copy the motor parameter table");
        this->addOperation("printTable", &Device::printTable, this,
                        RTT::OwnThread).doc("Print the motor parameter table");
        this->addOperation("setTable", &Device::setTable, this,
              RTT::OwnThread).doc("Set a value in the motor parameter table")
                             .arg("position", "Position to change")
                             .arg("value", "Value to assign at this position");
    }

    ACES::Word<RobotisPacket>* Device::processDS(ACES::Word<float>* w){
        if(w){
            RobotisPacket p;
            p.setID(credentials->getDevID());
            p.parameters = new std::deque<unsigned char>;
            float sp = 0.;
            switch( w->getMode() ){
                case(ACES::REFRESH):
                    //Indicate that the Device has sent a request, others must
                    //wait
                    requestPos = findTrueNodeID(w->getNodeID(), DOWN);
                    requestLen = PARAM_LEN[requestPos];
                    DSlockout = true;

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
        if(*(w->getCred()) == *credentials){
            RobotisPacket* p = &(w->getData());

            for(int i = 0, j=0; i < requestLen; j=i ){
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
                        memTable[requestPos+i] = low;
                        memTable[requestPos+i+1] = high;
                        i+=2;
                        break;
                    case 1:
                        tentry = (*(p->parameters))[i];
                        memTable[requestPos+i] = (*(p->parameters))[i];
                        i++;
                        break;
                    default:
                        assert(false);
                        break;
                }
                int node = findTrueNodeID(requestPos+j, UP);
                //The scaling function
                float data = USScale(tentry, node);
                ACES::Word<float> *res = new ACES::Word<float>(data, node,
                                                     credentials->getDevID(),
                                                     ACES::REFRESH,
                                                     credentials);
                res->printme();
                txUpStream.write(res);
            }
        }
        DSlockout = false;
        return NULL;
    }

    bool Device::getTable(){
        RobotisPacket p;
        p.setID(credentials->getDevID());
        p.parameters = new std::deque<unsigned char>;
        float sp = 0.;

        //Indicate that the Device has sent a request, others must
        //wait
        requestPos = 0;
        requestLen = 50;
        DSlockout = true;
        p.setInst(READ);
        p.parameters->push_back((unsigned char) requestPos);
        p.parameters->push_back((unsigned char) requestLen);

        ACES::Word<RobotisPacket> *pw = new ACES::Word<RobotisPacket>(p);
        txDownStream.write(pw);
        return true;
    }       

    void Device::printTable(){
        RTT::Logger::log(RTT::Logger::Info) << "Memory Table: "
                                            << RTT::endlog();
            for(int i=0; i < 10; i++){
                for(int j=0; j<5; j++){
                   RTT::Logger::log(RTT::Logger::Info) << std::setw(3) 
                           << (int)memTable[i*5+j] << ", ";
                }
                    RTT::Logger::log(RTT::Logger::Info) << RTT::endlog();
            }
    }

    bool Device::setTable(int position, float val){
        RobotisPacket p;
        p.setID(credentials->getDevID());
        p.parameters = new std::deque<unsigned char>;

        p.setInst(WRITE);
        p.parameters->push_back(position);
        
        //Scale the set-point
        unsigned short ssp = DSScale(val, position); 
        RTT::Logger::log() << "setpoint " << ssp << RTT::endlog();
        //Byte-chop the scaled point and add it to the param list
        appendParams(p.parameters, ssp,
                     PARAM_LEN[ position ] );

        ACES::Word<RobotisPacket> *pw = new ACES::Word<RobotisPacket>(p);
        txDownStream.write(pw);

        return true;
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
                           << " Zero=" << zero
                           << " Direction=" << direction << RTT::endlog();
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

        //Build packet for sending
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

    float Device::USScale(unsigned short in, int nodeID){
        float result = 0.;
        Credentials* c = (Credentials*)credentials;
        switch(nodeID){
            case GOAL_POSITION:
                result = in*(300./1023.)*(3.14159/180.0);
                result = limit<float>(result, 0., 5.2360);
                if(c->zero){
                    result = (result-(c->zero))*(c->direction);
                }
                break;
            default:        //case of no scaling function
                result = (float)in;
                break;
        }
        return result;
    }

    unsigned short Device::DSScale(float in, int nodeID){
        unsigned short result = 0;
	float temp = 0;
        Credentials* c = (Credentials*)credentials;
        switch(nodeID){
            case LED:
                result = (unsigned short)in;
                result = limit<unsigned short>(result, 0, 1);
                break;
            case CW_COMPLIANCE_MARGIN:
                //Intentional fall through
            case CCW_COMPLIANCE_MARGIN:
                result = (unsigned short)in;
                result = limit<unsigned short>(result, 0, 254);
                break;
            case CW_COMPLIANCE_SLOPE:
                //Intentional fall through
            case CCW_COMPLIANCE_SLOPE:
                result = (unsigned short)in;
                result = limit<unsigned short>(result, 1, 254);
                break;
            case GOAL_POSITION:
                //TODO - needs proper rounding
                if(c->zero){
                    temp = ((c->direction)*in)+(c->zero);
                }else{
                    temp = in;
		}
                result = (unsigned short)(temp*(1023./300.)*(180.0/3.14159));
                result = limit<unsigned short>(result, 0, 1023);
                break;
            case PUNCH:
                result = (unsigned short)in;
                result = limit<unsigned short>(result, 32, 1023);
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

    int findTrueNodeID(int id, DIRECTION d){
        //We want to be able to deal with position as though it were one node,
        //but get and set operations deal with two different positions in the
        //table, adjust pres_posit so they're the same.
        if(d == UP){
            switch(id){
                case PRESENT_POSITION:
                    return GOAL_POSITION;
                    break;
            }
        }
        if(d == DOWN){
            switch(id){
                case GOAL_POSITION:
                    return PRESENT_POSITION;
                    break;
            }
        }
        return id;
    }
};
