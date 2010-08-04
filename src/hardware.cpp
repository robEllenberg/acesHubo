#include "hardware.hpp"

namespace ACES {

    Hardware::Hardware(std::string cfg, std::string args) :
        taskCfg(cfg),
        RTT::TaskContext(name),
        announceRx("announceRx")
    {
        this->events()->addEvent(&announceRx, "announceRx", "word",
                                 "Recieved Data");

        this->setActivity( new RTT::Activity( priority, 1.0/freq, 0,
                                              name));
    }
    
    Hardware::Hardware(std::string name)
      : RTT::TaskContext(name)
    {
         this->events()->addEvent(&announceRx, "announceRx", "word",
                                 "Recieved Data");
    }

    bool Hardware::subscribeProtocol(Protocol* p){
        this->connectPeers( (RTT::TaskContext*) p);
        RTT::Handle h = p->events()->setupConnection("issueMessage")
            .callback( this, &Hardware::transmit
                       ,p->engine()->events() ).handle();
            //).handle();
        if(! h.ready() ){
            return false;
        }
        h.connect();
        if(!h.connected() ){
            return false;
        }

        h = this->events()->setupConnection("announceRx")
            .callback( p, &Protocol::interpretRx,
                       this->engine()->events() ).handle();

        if(! h.ready() ){
            return false;
        }
        h.connect();
        if(!h.connected() ){
            return false;
        }
 
        return true;
    }

/*
    charDevHardware::charDevHardware(std::string name,
                       std::ifstream *in,
                       std::ofstream *out, int priority,
                       int UpdateFreq) : 
            Hardware(name, priority, UpdateFreq),
            outBuffer("Out", 10),
            inBuffer("In")
        {
            hardpoint_in = in;
            hardpoint_out = out;
        };

    void charDevHardware::updateHook(){
        this->checkForLineData();
        this->checkForProtocolData();
    }

    void charDevHardware::checkForProtocolData(){
        unsigned char c;
        unsigned char count = 0;
        Message* cpv;
        while( this->inBuffer.size() and (count < 8) ){
            this->inBuffer.Pop(cpv);
            //RTT::Logger::log() << "Protocol Sent: "
            //<< c << std::endl;

            //this->hardpoint_out->put(
            //        ((Message*)cpv)->cval);
            count++;
        }
        this->hardpoint_out->flush();
    }

    void charDevHardware::checkForLineData(){
        unsigned char c;
        while( this->hardpoint_in->readsome((char*)&c, 1) ){
            //TODO - Reimplement, post removal of SVal's
            //charDevSValue* cpv = new charDevSValue(c);
            //outBuffer.buffer()->Push((Message*)cpv);	
	    }
    }
*/
}
