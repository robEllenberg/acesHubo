#include "hardware.hpp"

namespace ACES {
    Hardware::Hardware(std::string name, int pri,
                     int UpdateFreq) :
	    RTT::TaskContext(name),
	    NewData("NewData"),
	    outBuffer((std::string)"ToProtocol", 500),
        inBuffer((std::string)"FromProtocol")
        {
            priority = pri;
            frequency = UpdateFreq;
            this->events()->addEvent( &NewData,
                "Notify of new data arrival", "data",
                "The recieved data.");
            this->ports()->addPort( &outBuffer,
                "Ouput buffer to the supporting protocol");
            this->ports()->addPort( &inBuffer,
                "Input buffer from the supporting protocol");
            this->setActivity(
                new RTT::Activity(priority, 1.0/UpdateFreq));
            //this->methods()->addMethod( &isBusyMethod,
            //    "Check if the hardware is busy");
        }
    
    bool Hardware::configureHook(){
        return true;
    }

    bool Hardware::startHook(){
        unsigned char c;
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
        while( inBuffer.size() ){
        //RTT::Logger::log(RTT::Logger::Debug) << "Sizeof buffer:"
        // <<  inBuffer.size() << RTT::endlog();
            Message* m;
            inBuffer.Pop(m);
            transmit(m);
            //delete m;
            //TODO - add logic for managing responses from hw
        }
        recieve();
    }

    charDevHardware::charDevHardware(std::string name,
                       std::ifstream *in,
                       std::ofstream *out, int priority,
                       int UpdateFreq) : 
            Hardware(name, priority, UpdateFreq)
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
            //RTT::Logger::log() << "on-line: "
            //<<std::hex << (int)c << std::dec << std::endl;
            charDevPValue* cpv = new charDevPValue(c);
            outBuffer.buffer()->Push((Message*)cpv);	
	    }
    }
}
