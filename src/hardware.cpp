#include "hardware.hpp"

namespace ACES {


    template <class T>
    bool Hardware<T>::rxBus(){
        return true;
    }

    template <class T>
    bool Hardware<T>::subscribeProtocol(Protocol<T>* p){
        this->connectPeers( (RTT::TaskContext*) p);
        RTT::Handle h = p->events()->setupConnection("txDownStream")
            .callback( this, &Hardware::rxDownStream
            //           ,p->engine()->events() ).handle();
            ).handle();
        if(! h.ready() ){
            return false;
        }
        h.connect();
        if(!h.connected() ){
            return false;
        }

        h = this->events()->setupConnection("txUpStream")
            .callback( p, &Protocol<T>::rxUpStream
            //           ,this->engine()->events() ).handle();
            ).handle();

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
