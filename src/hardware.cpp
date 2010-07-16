#include "hardware.hpp"

namespace ACES {
    Hardware::Hardware(taskCfg cfg, std::string args) :
        taskCfg(cfg),
        RTT::TaskContext(name)
    {
        this->setActivity( new RTT::Activity( priority, 1.0/freq, 0,
                                              name));
    }
    
    bool Hardware::configureHook(){
        return true;
    }

    bool Hardware::startHook(){
        //TODO - Re-add a check to clear a non-empty buffer
        //before starting operation
        //while( this->hardpoint_in->readsome((char*)&c, 1) ){}
        return true;
    }

    void Hardware::stopHook(){
        
    }

    void Hardware::cleanupHook(){
    }


    void Hardware::updateHook(){
        //std::cout << "Update" << std::endl;
        //RTT::Logger::log(RTT::Logger::Debug) << "Update"
        //  << RTT::endlog();
/*
        while( inBuffer.size() ){
            Message* m;
            inBuffer.Pop(m);
            transmit(m);

            //delete m;
            //TODO - add logic for managing responses from hw
        }
*/
//        recieve();
    }

    bool Hardware::subscribeProtocol(Protocol* p){
        this->connectPeers( (RTT::TaskContext*) p);
        RTT::Handle h = p->events()->setupConnection("issueMessage")
            .callback( this, &Hardware::transmit
           //            hw->engine()->events() ).handle();
            ).handle();
        if(not h.ready() ){
            return false;
        }
        h.connect();
        if( not h.connected() ){
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
