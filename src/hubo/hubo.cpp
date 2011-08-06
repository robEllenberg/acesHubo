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

#include "hubo.hpp"

namespace Hubo{
    MotorCredentials::MotorCredentials(int board, int chan)
     : ACES::Credentials(board)
    {
        if(chan > 0 && chan <=ctrlSize){
            channels = chan;
        } else{
            RTT::Logger::log(RTT::Logger::Warning) <<  "Invalid channel number "
                << chan << " specified. Must be [1-" << ctrlSize <<"]."
                << RTT::endlog();
        }
        for(int i = 0; i < ctrlSize; i++){
            direction[i] = 1.;
            driveTeeth[i] = 1;
            drivenTeeth[i] = 1;
            encoderSize[i] = 0;
            zeroTicks[i] = 0;
            zeroCCW[i] = false;
            harmonic[i] = 0;
        }
    }

    ACES::Credentials* MotorCredentials::makeCredentials(std::string args){
        std::istringstream s(args);
        //TODO: hex interp
        int board, channels;
        s >> std::setbase(16) >> board >> std::setbase(10) >> channels;
        return (ACES::Credentials*)new MotorCredentials(board, channels);
    }

    int MotorCredentials::getChannels(){
        return channels;
    }

    bool MotorCredentials::setDirection(int chan, float dir){
        if(checkChannel(chan)){
            direction[chan] = dir;
            return true;
        }
        return false;
    }

    bool MotorCredentials::setGearRatio(int chan, int drive, int driven){
        if(checkChannel(chan)){
            driveTeeth[chan] = drive;
            drivenTeeth[chan] = driven;
            return true;
        }
        return false;
    }

    bool MotorCredentials::setEncoderSize(int chan, int size){
        if(checkChannel(chan)){
            encoderSize[chan] = size;
            return true;
        }
        return false;
    }

    bool MotorCredentials::setZero(int chan, int ticks, bool ccw){
        if(checkChannel(chan)){
            zeroTicks[chan] = ticks;
	    zeroCCW[chan] = ccw;
            return true;
        }
        return false;
    }

/*
    bool MotorCredentials::setOffsetPulse(int chan, int offset){
        if(checkChannel(chan)){
            offsetPulse[chan] = offset;
            return true;
        }
        return false;
    }

    bool MotorCredentials::setRevOffset(int chan, int offset){
        if(checkChannel(chan)){
            revOffset[chan] = offset;
            return true;
        }
        return false;
    }

    bool MotorCredentials::setCCW(int chan, bool ccw){
        if(checkChannel(chan)){
            CCW[chan] = ccw;
            return true;
        }
        return false;
    }
*/
    bool MotorCredentials::setHarmonic(int chan, int harm){
        if(checkChannel(chan)){
            harmonic[chan] = harm;
            return true;
        }
        return false;
    }

    float MotorCredentials::getDirection(int chan){
        if((chan < channels) and (chan >= 0)){
            return direction[chan];
        }
        return 0.0;
    }

    unsigned int MotorCredentials::getEncoderSize(int chan){
        if((chan < channels) and (chan >= 0)){
            return encoderSize[chan];
        }
        return 0;
    }

/*
    int MotorCredentials::getOffsetPulse(int chan){
        if((chan < channels) and (chan >= 0)){
            return offsetPulse[chan];
        }
        return 0;
    }

    int MotorCredentials::getRevOffset(int chan){
        if((chan < channels) and (chan >= 0)){
            return revOffset[chan];
        }
        return 0;
    }

    bool MotorCredentials::getCCW(int chan){
        if((chan < channels) and (chan >= 0)){
            return CCW[chan];
        }
        return false;
    }
*/
    unsigned int MotorCredentials::getHarmonic(int chan){
        if((chan < channels) and (chan >= 0)){
            return harmonic[chan];
        }
        return 0;
    }

    float MotorCredentials::getGearRatio(int chan){
        if((chan < channels) and (chan >= 0)){
            return (float)drivenTeeth[chan]/(float)driveTeeth[chan];
        }
        return 0;
    }

    int MotorCredentials::getZeroTicks(int chan){
        if(checkChannel(chan)){
            return zeroTicks[chan];
        }
        return 0;
    }

    bool MotorCredentials::getZeroCCW(int chan){
        if(checkChannel(chan)){
            return zeroCCW[chan];
        }
        return false;
    }

    bool MotorCredentials::checkChannel(int chan){
        if(chan >= 0 && chan < channels){
            return true;
        }
        else{
            RTT::Logger::log(RTT::Logger::Warning) <<  "Invalid channel number "
                << chan << " specified. Must be [0-" << channels-1 << "]."
                << RTT::endlog();
        }
        return false; 
    }

    void MotorCredentials::printme(){
        ACES::Credentials::printme();
        RTT::Logger::log() << "(HuboDriver) Credentials: \n"
                           << "# Channels = " << channels << "\n";
        
        RTT::Logger::log() << "Direction: [";
        for(int i = 0; i < ctrlSize; i++){
            RTT::Logger::log() << direction[i] << ", ";
        }
        RTT::Logger::log() << "]\nEncoder Size: [";
        for(int i = 0; i < ctrlSize; i++){
            RTT::Logger::log() << encoderSize[i] << ", ";
        }
        RTT::Logger::log() << "]\nZeroTicks: [";
        for(int i = 0; i < ctrlSize; i++){
            RTT::Logger::log() << zeroTicks[i] << ", ";
        }
        RTT::Logger::log() << "]\nZeroCCW: [";
        for(int i = 0; i < ctrlSize; i++){
            RTT::Logger::log() << zeroCCW[i] << ", ";
        }
        RTT::Logger::log() << "]\nGear Ratio: [";
        for(int i = 0; i < ctrlSize; i++){
            RTT::Logger::log() << (float)drivenTeeth[i]/(float)driveTeeth[i] << ", ";
        }
        RTT::Logger::log() << "]\nHarmonic: [";
        for(int i = 0; i < ctrlSize; i++){
            RTT::Logger::log() << harmonic[i] << ", ";
        }
        RTT::Logger::log() <<  "]" << RTT::endlog();
    }

    SensorCredentials::SensorCredentials(int boardID, int chan)
     :ACES::Credentials(boardID)
    {
        if(chan > 0 && chan <=senseSize){
            channels = chan;
        } else{
            RTT::Logger::log(RTT::Logger::Warning) <<  "Invalid channel number "
                << chan << " specified. Must be [1-" << ctrlSize <<"]."
                << RTT::endlog();
        }
        for(int i = 0; i < senseSize; i++){
            direction[i] = 1.;
            scale[i] = 1.;
        }
    }

    ACES::Credentials* SensorCredentials::makeCredentials(std::string args){
        std::istringstream s(args);
        int id, channels;
        s >> std::setbase(16) >> id >> std::setbase(10) >> channels;
        return (ACES::Credentials*)new SensorCredentials(id, channels);
    }

    int SensorCredentials::getChannels(){
        return channels;
    }

    bool SensorCredentials::checkChannel(int chan){
        if(chan >= 0 && chan < channels){
            return true;
        }
        else{
            RTT::Logger::log(RTT::Logger::Warning) <<  "Invalid channel number "
                << chan << " specified. Must be [0-" << channels-1 << "]."
                << RTT::endlog();
        }
        return false; 
    }

    float SensorCredentials::getScale(int chan){
        if((chan < channels) and (chan >= 0)){
            return scale[chan];
        }
        return 0.0;
    }

    float SensorCredentials::getDirection(int chan){
        if((chan < channels) and (chan >= 0)){
            return direction[chan];
        }
        return 0.0;
    }
    
    void SensorCredentials::printme(){
        ACES::Credentials::printme();
        RTT::Logger::log() << "(HuboSensor) Credentials: \n"
                           << "# Channels = " << channels << "\n";
        
        RTT::Logger::log() << "Direction: [";
        for(int i = 0; i < senseSize; i++){
            RTT::Logger::log() << direction[i] << ", ";
        }
        RTT::Logger::log() << "]\nScale: [";
        for(int i = 0; i < senseSize; i++){
            RTT::Logger::log() << scale[i] << ", ";
        }
        RTT::Logger::log() <<  "]" << RTT::endlog();
    }

    bool SensorCredentials::setDirection(int chan, float dir){
        if(checkChannel(chan)){
            direction[chan] = dir;
            return true;
        }
        return false;
    }

    bool SensorCredentials::setScale(int chan, float sca){
        if(checkChannel(chan)){
            scale[chan] = sca;
            return true;
        }
        return false;
    }

    CANHardware::CANHardware(std::string cfg, std::string args)
      : ACES::Hardware<canmsg_t*>(cfg, args)
    {
        //args contains the CAN device we're interested in e.g. "/dev/can0"
        //the baud rate to operate at 
        // args = "/dev/someFD <(int)baudrate>"
        std::istringstream s(args);
        s >> fd >> rate;

        this->addAttribute("rate", rate);
        this->addAttribute("fileNode", fd);

        //Add a loopback connection that the device can use to add to its own
        //queue
        this->ports()->addPort("TxDSLoopback", txDSLoop).doc(
                               "Downstream (to Bus) Self Connection");
        RTT::base::PortInterface *tx = NULL, *rx=NULL;
        RTT::ConnPolicy policy = RTT::ConnPolicy::buffer(200);
        tx = (RTT::base::PortInterface*)this->ports()->getPort("TxDSLoopback");
        rx = (RTT::base::PortInterface*)this->ports()->getPort("RxDS");
        bool success = tx->connectTo(rx, policy);
        assert(success);
        
        this->addOperation("genPacket", &CANHardware::genPacket, this,
                               RTT::OwnThread).doc("Transmit a packet on the"
                               " CAN line")
                               .arg("ID", "Message ID")
                               .arg("len", "Message Length")
                               .arg("packet", "Hex string containing packet content");
    }

    bool CANHardware::startHook(){

        #if TESTMODE == 1
            beginning = RTT::os::TimeService::Instance()->getTicks();
            std::string ofd = fd + ".out";
            channel = open(ofd.c_str(), O_WRONLY | O_CREAT | O_TRUNC,
                           0655);
            assert(channel >= 0 );
            // If we're in offline operation, we need to open in the input file
            // and set it up for reading
            std::string ifd = fd + ".in";
            ichannel.open(ifd.c_str());

            char linebuf[100];
            for(int i = 0; i < 4; i++){
                //Read & discard the CAN sniff header which is 4 lines
                ichannel.getline(linebuf, 100);
            }
        #else //Normal operation action
            //Begin - snippit from can4linux examples baud.c
            channel = open(fd.c_str(), O_RDWR | O_NONBLOCK );
            Config_par_t  CANcfg;
            Command_par_t cmd;

            cmd.cmd = CMD_STOP;
            ioctl(channel, CAN_IOCTL_COMMAND, &cmd);

            CANcfg.target = CONF_TIMING; 
            CANcfg.val1   = rate;
            ioctl(channel, CAN_IOCTL_CONFIG, &CANcfg);

            cmd.cmd = CMD_START;
            ioctl(channel, CAN_IOCTL_COMMAND, &cmd);
            //End snippit
        #endif
        return true;
    }
    
    void CANHardware::stopHook(){
        close(channel);
        #if TESTMODE == 1
            ichannel.close();
        #endif
    }

    bool CANHardware::genPacket(int ID, int len, 
                                std::string packetContent)
    {
        ACES::Message<canmsg_t*> *m = new ACES::Message<canmsg_t*>();
        unsigned long long int packet = 0ULL;
        std::istringstream pacContStream(packetContent);
        pacContStream >> std::setbase(16) >> packet;

        canmsg_t* msg = new canmsg_t;
        msg->id = ID;
        msg->cob = 0;
        msg->flags = 0;
        msg->length = len;
        switch(len){
            case 8:
                msg->data[7] = ((packet & 0x00000000000000FFULL) >> (8*0));
            case 7:
                msg->data[6] = ((packet & 0x000000000000FF00ULL) >> (8*1));
            case 6:
                msg->data[5] = ((packet & 0x0000000000FF0000ULL) >> (8*2));
            case 5:
                msg->data[4] = ((packet & 0x00000000FF000000ULL) >> (8*3));
            case 4:
                msg->data[3] = ((packet & 0x000000FF00000000ULL) >> (8*4));
            case 3:
                msg->data[2] = ((packet & 0x0000FF0000000000ULL) >> (8*5));
            case 2:
                msg->data[1] = ((packet & 0x00FF000000000000ULL) >> (8*6));
            case 1:
                msg->data[0] = ((packet & 0xFF00000000000000ULL) >> (8*7));
            case 0:
                break;
            default:
                assert(0);
        }
        m->push( new ACES::Word<canmsg_t*>(msg) );

        txDSLoop.write(m);
        return true;
    }

    bool CANHardware::txBus(ACES::Message<canmsg_t*>* m){
        while( m->size() ){
            ACES::Word<canmsg_t*>* w = m->pop();
            canmsg_t* msg = w->getData();
            int r = 0;
            #if TESTMODE == 1
                //Grab the current time and copy it to a string
                RTT::Seconds sampleTime =
                    RTT::os::TimeService::Instance()->secondsSince(beginning);
                ostringstream s (ostringstream::out);
                s << sampleTime;

                //Print timestamp, id, and length
                dprintf(channel, "%s\t\t%2.2X\t%d\t\t", s.str().c_str(),
                        (int)(msg->id), msg->length);
                //Print the contents of the data buffer
                for(int i = 0; i < msg->length; i++){
                    dprintf(channel, "%2.2X ", (msg->data[i]) );
                }
                dprintf(channel, "\n");
                r = 1;  //Bypass the error checks later
                //int r = write(channel, &(msg->data), msg->length);
            #else //Normal operation
                r = write(channel, msg, 1);
            #endif 
            if(r == -1){
                RTT::Logger::log() << RTT::Logger::Info
                << "CAN Transmission Write Error"
                << RTT::endlog();
            } else if(r == 0){
                RTT::Logger::log() << RTT::Logger::Info
                << "CAN Transmission Timeout"
                << RTT::endlog();
            }
            //delete msg;
            //TODO - Delete the word
        }
        //delete m;
        return true;
    }

    void CANHardware::rxBus(int size){
        float time = 0;
        int chan = 0, len = 0, rxSize = 0;
        unsigned long msgID = 0;
        std::string junk;

        #if TESTMODE == 1 //Offline operation

            char linebuf[100];
            while( (not ichannel.eof()) and (rxSize < canBuffSize)){
                ichannel.getline(linebuf, 100);
                std::string buf(linebuf);
                std::istringstream s(buf);
                s >> time >> chan >> std::setbase(16)
                  >> msgID >> junk >> junk >> len;

                rxBuffer[rxSize].id = msgID;
                rxBuffer[rxSize].length = len;
                //switch stream to hex mode
                for(int i = 0, temp = 0; i < len; i++){
                    s >> std::setbase(16) >> temp;
                    rxBuffer[rxSize].data[i] = temp;
                }
                rxSize++;
            }
        #else             //Online operation
            rxSize = read(channel, &rxBuffer, canBuffSize);
        #endif

        ostringstream packet (ostringstream::out);
        for(int i = 0; i < rxSize; i++){
            if(i == 0){
                packet << "Reception of size " << rxSize << ": ";
            }
            canmsg_t* msg = new canmsg_t;
            msg->flags = rxBuffer[i].flags;
            msg->cob = rxBuffer[i].cob;
            msg->id = rxBuffer[i].id;
            msg->timestamp = rxBuffer[i].timestamp;
            msg->length = rxBuffer[i].length;
            for(int j = 0; j < msg->length; j++){
                msg->data[j] = rxBuffer[i].data[j];
                packet << "0x" << std::setbase(16) << (int)(msg->data[j]) << ", ";
            }
            packet << std::endl;
            ACES::Word<canmsg_t*>* w = new ACES::Word<canmsg_t*>(msg);
            txUpStream.write(w);

            if(i == rxSize-1){
                packet << "$";
                RTT::Logger::log(RTT::Logger::Debug) << packet.str() << RTT::endlog();
            }
        }
    }

/*
    int CANHardware::readFDline(int fd, char* buffer, int maxlen){
        int ret = 0;
        for(int i = 0; i < maxlen-1; i++){
            ret = read(fd, buffer+i, 1);
            if(ret <= 0){
                return ret;
            }
            if(buffer[i] == '\n'){
                buffer[i+1] = 0;
                return i;
            }
        }
        return -1;
    }
*/

    Protocol::Protocol(std::string cfg, std::string args)
      : ACES::Protocol<canmsg_t*, canMsg>(cfg, args)
    {}

    ACES::Credentials* Protocol::credFromPacket(canMsg& c){
        return new ACES::Credentials(c.getID());
    }

    ACES::Message<canmsg_t*>* Protocol::processDS(ACES::Word<canMsg>* h){
        ACES::Message<canmsg_t*> *m = NULL;
        if(h){
            canmsg_t* msg = h->getData().toLineType();
            m = new ACES::Message<canmsg_t*>;
            m->push( new ACES::Word<canmsg_t*>(msg) );
        }
        return m;
    }

    ACES::Word<canMsg>* Protocol::processUS(ACES::Word<canmsg_t*>* usIn){
        ACES::Word<canMsg>* pw = NULL;
        canmsg_t* c = usIn->getData();
        unsigned long r1 = 0, r2 = 0, r3 = 0, r4 = 0, r5 = 0;
        huboCanType idClass = CAN_NONE;

        if(c->id < 0x40){
            //anything below id 0x40 is outgoing, so we can ignore it
            return NULL;
        }

        //The equalizer is needed because the offsets used to differentiate
        //between motor controllers are inconsistant throughout the protocol,
        //specfically with respect to the FT/ADC sensors. For the main
        //identifier (NAME#_RXDF) subtracting the category value (0x190) leaves
        //you with the board number. For the sensor identifiers
        //(SENSOR_FT/AD#_RXDF) subtracting the category values (0x40/0x40)
        //leaves you with a 1-indexed arbitrary sensor number which does not
        //match the board number. Equalizer is used to correct this, so that
        //the board number is always returned on the canMsg.
        int equalizer = 0;

        if( offsetRange(NAME_RXDF, c->id) ){
            idClass = NAME_RXDF;
            r1 = c->data[0]; // Time?
            r2 = c->data[1]; // SearchLimit0 - Corrupted
            r3 = c->data[2]; // SearchLimit1 - Corrupted
        }
        else if( offsetRange(STAT_RXDF, c->id) ){
            idClass = STAT_RXDF;
            r1 = c->data[0];                            //JmcSTATbyte
            r2 = assemble2Byte(c->data[1], c->data[2]);//(m0STATbyte, m1STATbyte)
            r3 = assemble2Byte(c->data[3], c->data[4]);//(m0CTLbyte, m1CTLbyte)
            r4 = assemble2Byte(c->data[5], c->data[6]);//[CtrlStat1, CtrlStat0]
        }
        else if( offsetRange(SENSOR_FT_RXDF, c->id)){
            idClass = SENSOR_FT_RXDF;
            equalizer = 0x2F;
            r1 = assemble2Byte(c->data[0], c->data[1]); //Mx
            r2 = assemble2Byte(c->data[2], c->data[3]); //My
            r3 = assemble2Byte(c->data[4], c->data[5]); //Fz
        }
        else if( offsetRange(SENSOR_AD_RXDF, c->id)){
            idClass = SENSOR_AD_RXDF;
            equalizer = 0x2F;
            r1 = assemble2Byte(c->data[0], c->data[1]); //Acc1
            r2 = assemble2Byte(c->data[2], c->data[3]); //Acc2
            r3 = assemble2Byte(c->data[4], c->data[5]); //Gyro1
            r4 = assemble2Byte(c->data[6], c->data[7]); //Gyro2
        }
        else if( offsetRange(DAOFFSET_RXDF, c->id)){
            idClass = DAOFFSET_RXDF;
            r1 = assemble2Byte(c->data[0], c->data[1]); //Mx
            r2 = assemble2Byte(c->data[2], c->data[3]); //My
            r3 = assemble2Byte(c->data[4], c->data[5]); //Fz
        }
        else if( offsetRange(ADOFFSET_RXDF, c->id)){
            idClass = ADOFFSET_RXDF;
            r1 = assemble2Byte(c->data[0], c->data[1]); //Mx
            r2 = assemble2Byte(c->data[2], c->data[3]); //My
            r3 = assemble2Byte(c->data[4], c->data[5]); //Fz
        }
        else if( offsetRange(OFFSET_RXDF, c->id)){
            idClass = OFFSET_RXDF;
            r1 = assemble2Byte(c->data[0], c->data[1]); //AD
            r2 = assemble2Byte(c->data[2], c->data[3]); //DA
        } else {
            //printf(failed to match a packet)
            return pw;
        }

        int id = (c->id) - ((int)idClass) + equalizer;
        canMsg msg(id, idClass, CMD_NONE, r1, r2, r3, r4, r5);
        msg.printme();
        pw = new ACES::Word<canMsg>(msg, 0, 0, 0, Protocol::credFromPacket(msg));
        return pw;
    }

    /*! \param t Indicator of the range we wish to test against
        \param lineID The message ID used in the CAN packet

        Function is only useful when comparing the gyro/adc packets
    */
    bool Protocol::offsetRange(huboCanType t, int lineID){
        int baseval = (int)t;
        //All address ranges for these types of sensors are 0x10 wide
        switch( baseval ){
            case SENSOR_FT_RXDF:
            case SENSOR_AD_RXDF:
            case DAOFFSET_RXDF:
            case ADOFFSET_RXDF:
            case OFFSET_RXDF:
                //These classes of command have a width of 0x10
                if( (lineID >= baseval) and (lineID < baseval+0x10) ){
                    return true;
                } break;
            case ENC_RXDF:
            case CUR_RXDF:
            case PM_RXDF:
                //These classes of command have a width of 0x30
                if( (lineID >= baseval) and (lineID < baseval+0x30) ){
                    return true;
                } break;
            case NAME_RXDF:
            case STAT_RXDF:
                //These classes of command have a width of 0x40
                if( (lineID >= baseval) and (lineID < baseval+0x40) ){
                    return true;
                } break;
            default:
                assert(false);
        }
        return false;
    }

    unsigned long Protocol::assemble2Byte(unsigned char lsb, unsigned char msb){
        unsigned long t = 0;
        t |= msb;
        t <<= 8;
        t |= lsb;
        return t;
    }

    /*
    unsigned long Protocol::assemble4Byte(unsigned char b1, unsigned char b2)
    */

    HuboDevice::HuboDevice(std::string cfg, std::string args)
     :ACES::Device<float, canMsg>(cfg)
    {}

    ACES::Word<canMsg>* HuboDevice::buildWord(canMsg c, int channel){
        MotorCredentials* cred = (MotorCredentials*)credentials;
        return new ACES::Word<canMsg>(c, channel, cred->getDevID(),
                                      ACES::SET, credentials);
    }

    MotorDevice::MotorDevice(std::string cfg, std::string args)
      : HuboDevice(cfg, args), instantTrigger(false)
    {
        credentials =
            (ACES::Credentials*)MotorCredentials::makeCredentials(args);

        this->addOperation("setDirection", &MotorDevice::setDirection, this,
            RTT::OwnThread).doc("Set the direction of rotation for a channel")
                           .arg("channel", "Channel number to set")
                           .arg("dir", "Direction of rotation = +/-1");

        this->addOperation("setGearRatio", &MotorDevice::setGearRatio, this,
            RTT::OwnThread).doc("Set the gear ratio for a channel")
                           .arg("channel", "Channel number to set")
                           .arg("drive", "Number of teeth on the drive axis")
                           .arg("driven", "Number of teeth on the driven axis");

        this->addOperation("setEncoderSize", &MotorDevice::setEncoderSize, this,
            RTT::OwnThread).doc("Set the gear ratio for a channel")
                           .arg("channel", "Channel number to set")
                           .arg("size", "Number of ticks on the encoder");

        this->addOperation("setGains", &MotorDevice::setGains, this,
            RTT::OwnThread).doc("Set the gains for this controller")
                           .arg("type", "Type of gain - Position or Torque")
                           .arg("channel", "Channel number to set")
                           .arg("Kp", "Proportional Gain")
                           .arg("Ki", "Integral Gain")
                           .arg("Kd", "Derivative Gain");

        this->addOperation("programZero", &MotorDevice::programZero, this,
            RTT::OwnThread).doc("Set the harmonic drive ratio.")
                           .arg("channel", "Channel number to set");

        this->addOperation("setZero", &MotorDevice::setZero, this,
            RTT::OwnThread).doc("Set the harmonic drive ratio.")
                           .arg("channel", "Channel number to set")
                           .arg("ticks", "Number of ticks for zero")
                           .arg("ccw", "true - counter-clockwise, "
                                "false - clockwise");

        this->addOperation("setHarmonic", &MotorDevice::setHarmonic, this,
            RTT::OwnThread).doc("Set the harmonic drive ratio.")
                           .arg("channel", "Channel number to set")
                           .arg("harmonic", "Drive ratio");

        this->addOperation("HIPenable", &MotorDevice::setHIPenable, this,
            RTT::OwnThread).doc("Send a packet to the controller enabling"
                                " the HIP");

        this->addOperation("run", &MotorDevice::setRunCmd, this,
            RTT::OwnThread).doc("Send a packet to the controller to activate it"
                               );

        this->addAttribute("instantTrigger", instantTrigger);
    }

    ACES::Word<canMsg>* MotorDevice::processDS(ACES::Word<float>* w){
        ACES::Word<canMsg>* msg = NULL;
        if(w and (w->getMode() == ACES::SET) ){
            if( applySetPoint(w->getNodeID(), w->getData(), instantTrigger) ){
                canMsg c = buildSetPacket();
                msg = buildWord(c, 0);
            }
        }
        //RTT::Logger::log(RTT::Logger::Warning) <<  "Processing: " << msg
        //<< RTT::endlog();
        return msg;
    }

    ACES::Word<float>* MotorDevice::processUS(ACES::Word<canMsg>* msg){
        if(*(msg->getCred()) == *credentials){
            canMsg c = msg->getData();
            //The behaviors are too complex to handle all here, so we break
            //them out based on the type of packet being received and use a
            //specific processing function for each.
            switch(c.getType()){
                case SENSOR_FT_RXDF:
                case SENSOR_AD_RXDF:
                    //These cases handled by the sensor device
                case DAOFFSET_RXDF:
                case ADOFFSET_RXDF:
                case OFFSET_RXDF:
                    //TODO - WTF are these packets for?
                    break;
                case ENC_RXDF:
                case CUR_RXDF:
                case PM_RXDF:
                    //TODO - HuboLab never uses them, I don't see why we would
                    break;
                case NAME_RXDF:
                    processUS_NAME_RXDF(msg);
                    break;
                case STAT_RXDF:
                    processUS_STAT_RXDF(msg);
                    break;
                default:
                    break;
            }
            //TODO: delete msg->credentials
        }
        return NULL;
    }

    void HuboDevice::processUS_NAME_RXDF(ACES::Word<canMsg>* msg){
        //This packet appears to be total junk - doing nothing with it for now.
    }

    void HuboDevice::processUS_STAT_RXDF(ACES::Word<canMsg>* msg){
        canMsg c = msg->getData();
        //I'm not doing anything with these because they really don't seem to
        //contain any meaningful information. They're just being broken out so,
        //if they end up being important, someone can easily grab them. See the
        //hubo code base: BLDC_ReadCMD.c; Send_Status(void)
        unsigned char JmcSTATbyte = c.getR1();
        unsigned char m0STATbyte = c.bitStrip(c.getR2(), 0);
        unsigned char m1STATbyte = c.bitStrip(c.getR2(), 1);
        unsigned char m0CTLbyte = c.bitStrip(c.getR3(), 0);
        unsigned char m1CTLbyte = c.bitStrip(c.getR3(), 1);
        unsigned char m0Stat = c.bitStrip(c.getR4(), 0);
        unsigned char m1Stat = c.bitStrip(c.getR4(), 1);
                      
        //TODO - Make these messages more elaborate
        //0xA is a mask which reveals only the origin and limit state - see
        //hubo code
        if(m0Stat & 0xA){
            RTT::Logger::log(RTT::Logger::Warning)
                <<  "Failed to aquire origin/limit on Controller "
                << ", Channel 0"
                << RTT::endlog();
        }else if(m0Stat & 0x5){
            RTT::Logger::log(RTT::Logger::Warning)
                <<  "Received a clean ACK on " << name << " "
                << ", Channel 0"
                << RTT::endlog();
        }

        if(m1Stat & 0xA){
            RTT::Logger::log(RTT::Logger::Warning)
                <<  "Failed to aquire origin/limit on Controller "
                << ", Channel 1"
                << RTT::endlog();
        }else if(m1Stat & 0x5){
            RTT::Logger::log(RTT::Logger::Warning)
                <<  "Received a clean ACK on " << name << " "
                << ", Channel 1"
                << RTT::endlog();
        }
    }

    int MotorDevice::getChannels(){
        return ((MotorCredentials*)credentials)->getChannels();
    }

    bool MotorDevice::setDirection(int chan, float dir){
        return ((MotorCredentials*)credentials)->setDirection(chan, dir);
    }

    bool MotorDevice::setGearRatio(int chan, int drive, int driven){
        return ((MotorCredentials*)credentials)->setGearRatio(chan, drive, driven);
    }

    bool MotorDevice::setEncoderSize(int chan, int size){
        return ((MotorCredentials*)credentials)->setEncoderSize(chan, size);
    }

    bool MotorDevice::setHarmonic(int chan, int harmonic){
        return ((MotorCredentials*)credentials)->setHarmonic(chan, harmonic);
    }

    bool MotorDevice::setZero(int chan, int ticks, bool ccw){
        return ((MotorCredentials*)credentials)->setZero(chan, ticks, ccw);
    }

    bool MotorDevice::setSetPoint(int channel, float sp, bool instantTrigger){
        if(applySetPoint(channel, sp, instantTrigger)){
            canMsg c = buildSetPacket();
            ACES::Word<canMsg>* msg = buildWord(c, 0);
            txDownStream.write(msg);
            return true;
        }
        return false;
    } 

    /*! Applies a newly aquired set point to the device and checks if a full
     * set of points has been received. Returns true if full set available, and
     * false if a full set not available. instantTrigger can be used to always
     * return true.
     */
    bool MotorDevice::applySetPoint(int channel, float sp, bool instantTrigger)
    {
        int numChan = getChannels();
        if(channel >= 0 && channel < numChan){
            setPoint[channel] = sp;
            trigger[channel] = true;
            
            if(instantTrigger or triggersSet()){
                clearTrigger();
                return true;
            }
        }
        else{
            RTT::Logger::log(RTT::Logger::Warning) <<  "Invalid channel number "
                << channel << " specified. Must be [1-" << numChan << "]."
                << RTT::endlog();
        } 
        return false;
    }

    /*! Inform us if all channels have recieved new information and are ready
     *  to fire. */
    bool MotorDevice::triggersSet(){
        int numChan = getChannels();
        bool go = true;
        for(int i = 0; i < numChan; i++){
            go = go && trigger[i];
        }
        return go;
    }
    
    void MotorDevice::clearTrigger(){
        int numChan = getChannels();
        for(int i = 0; i < numChan; i++){
            trigger[i] = false;
        }
    }

    bool MotorDevice::setGains(std::string type, int channel, int Kp, int Ki, int Kd){
        cmdType t = CMD_NONE;
        if(type == "Position"){
            switch(channel){
                case 0:
                    t = SET_POS_GAIN_A;
                    break;
                case 1:
                case 2:
                    t = SET_POS_GAIN_B;
                    break;
                default:
                    RTT::Logger::log(RTT::Logger::Warning)
                        << "Invalid channel number "
                        << channel << " specified. Must be [0-"
                        << channel-1 << "]."
                        << RTT::endlog();
            }
        } else if(type == "Torque"){
            switch(channel){
                case 0:
                    t = SET_TRQ_GAIN_A;
                    break;
                case 1:
                case 2:
                    t = SET_TRQ_GAIN_B;
                    break;
                default:
                    RTT::Logger::log(RTT::Logger::Warning)
                        << "Invalid channel number "
                        << channel << " specified. Must be [0-"
                        << channel-1 << "]."
                        << RTT::endlog();
            }
        }
        else{
            RTT::Logger::log(RTT::Logger::Warning) <<  "Invalid gain type, \""
                << type << "\" specified. Must be \"Position\" or \"Torque\"."
                << RTT::endlog();
        }

        if(t){
            canMsg c = buildGainPacket(t, Kp, Ki, Kd);
            txDownStream.write( buildWord(c, channel) );
            return true;
        }
        return false;
    }

    bool MotorDevice::programZero(int chan){
        MotorCredentials* c = (MotorCredentials*)credentials;
        if(c->checkChannel(chan)){
            int ticks = c->getZeroTicks(chan);
            bool ccw = c->getZeroCCW(chan);
            canMsg c = buildZeroPacket(chan, ticks, ccw);
            txDownStream.write( buildWord(c, chan) );
            return true;
        }
        return false;
    }

    canMsg MotorDevice::buildZeroPacket(int c, int ticks, bool ccw){
        MotorCredentials* cred = (MotorCredentials*)credentials;
        long r1 = 0, r2 = 0, r3 = 0, r4 = 0, r5=0;

        //Check to make sure we're not exceding the value that can be held in
        //hubolab's 19bit + sign bit representation
        int nineteenMax = 0x7FFFF;
        if(ticks > nineteenMax){
            ticks = nineteenMax;
        }
        if(ticks < -nineteenMax){
            ticks = -nineteenMax;
        }

        //Convert to a sign-bit (as opposed to 2's complement) representation
        unsigned long sign = 0;
        if(ticks < 0){
            ticks *= -1;
            sign = 0x80000;
        }
        ticks |= sign;
        
        //The lowest bit in mode is determined by the rotational direction of
        //the motor. 1 for CCW, 0 for CW
        long mode = 0;
        if( ccw ){
                mode |= 1;
        }

        if( cred->getChannels() == 2 ){  //Two channel controller
            mode |= (1 << (4+c)); //add a flag to indicate the channel number
            r1 = mode;
            switch(c){
                case 0:
                    r2 = ticks & 0xFFFF;
                    r4 = (ticks & 0xF0000) >> 16;
                    break;
                case 1:
                    r3 = ticks & 0xFFFF;
                    r4 = (ticks & 0xF0000) >> 12;
                    break;
                default:
                    break;
            }
        } else if (cred->getChannels() == 3){  //Three channel controller
            r5 = 1;     //We use r5 as a flag to indicate a three channel
                        //controller to the lower level functions
            mode |= (0x10); //three channel boards always use this designation
            r2 = ((short)ticks & 0xFFFF);
            //TODO - Correct This
            r3 = 0x3232;
        } else{
            //Fail - we can't do this for other controller configurations
            RTT::Logger::log(RTT::Logger::Warning) << "Attempted to calibrate "
            "a board with improper channel setting. " << cred->getChannels() 
            << "channels were specified. Only 2 or 3 are allowed for this "
            "operation." << RTT::endlog();
        }

        return canMsg(cred->getDevID(), CMD_TXDF, GO_LIMIT_POS, r1, r2, r3, r4, r5);
    }
/*
    bool MotorDevice::setCalibrate(int channel){
        if( ((MotorCredentials*)credentials)->checkChannel(channel) ){
            canMsg c = buildCalibratePulse(channel);
            ACES::Word<canMsg>* msg = buildWord(c, channel);
            txDownStream.write(msg);
            return true;
        }
        return false;
    }
*/
    bool MotorDevice::setHIPenable(){
        canMsg c = buildHIPpacket();
        ACES::Word<canMsg>* msg = buildWord(c, 0);
        txDownStream.write(msg);
        return true;
    }

    bool MotorDevice::setRunCmd(){
        canMsg c = buildRunCmdPacket();
        ACES::Word<canMsg>* msg = buildWord(c, 0);
        txDownStream.write(msg);
        return true;
    }

    canMsg MotorDevice::buildGainPacket(cmdType type, int Kp, int Ki, int Kd){
        return canMsg(credentials->getDevID(), CMD_TXDF, type, Kp, Ki, Kd, 0, 0);
    }

    canMsg MotorDevice::buildSetPacket(){
        int numChan = getChannels(); 
        unsigned long temp[5];
        switch(numChan){
            float dir, ppr;
            case 5: //Five Channel Motor Controller - Fingers
                for(int i=0; i < 5; i++){
                    dir = ((MotorCredentials*)credentials)->getDirection(i);
                    ppr = ((MotorCredentials*)credentials)->getHarmonic(i)
                          *((MotorCredentials*)credentials)->getGearRatio(i)
                          *((MotorCredentials*)credentials)->getEncoderSize(i);
                    temp[i] =
                        canMsg::bitStuff15byte((long)((setPoint[i]*RAD2DEG)*dir*ppr/360.));
                }
                break;
            case 3: //Three Channel Motor Controllers - Wrists & Neck
                for(int i = 0; i < 3; i++){
                    dir = ((MotorCredentials*)credentials)->getDirection(i);
                    ppr = ((MotorCredentials*)credentials)->getHarmonic(i)
                          *((MotorCredentials*)credentials)->getGearRatio(i)
                          *((MotorCredentials*)credentials)->getEncoderSize(i);
                    temp[i] = (setPoint[i]*RAD2DEG)*dir*ppr/360.;
                }
                break;
            case 2: //Two Channel Motor Controllers - Limbs
                for(int i = 0; i < 2; i++){
                    dir = ((MotorCredentials*)credentials)->getDirection(i);
                    ppr = ((MotorCredentials*)credentials)->getHarmonic(i)
                          *((MotorCredentials*)credentials)->getGearRatio(i)
                          *((MotorCredentials*)credentials)->getEncoderSize(i);
                    temp[i] =
                       canMsg::bitStuff3byte((long)((setPoint[i]*RAD2DEG)*dir*ppr/360.));
                }
                break;
            default:
                //TODO - Failure case
                break;
        }

        canMsg cm( credentials->getDevID(),
                REF_TXDF, (cmdType)numChan, temp[0], temp[1], temp[2], temp[3],
                temp[4]);
        return cm;
    }

/*        
    canMsg MotorDevice::buildCalibratePulse(int c){ //c - channel number
        MotorCredentials* cred = (MotorCredentials*)credentials;
        long offset1 =0, offset2 = 0, offset3 = 0;
        canMsg msg;

        //The lowest bit in mode is determined by the rotational direction of
        //the motor 1 for CCW, 0 for CW
        long mode = 0;
        if(c > 1){c = 0;} //Treat the 3-channel motor specially

        if(cred->getCCW(c)){
            mode |= 1;
        }
        mode |= (1 << (4+c));

        switch(cred->getChannels()){   //The rest of the processing depends not on which
                            //channel we've selected, but on the number of
                            //channels on the controller
            case 2: //Hubo-Code equiv -> CalPulse()
                offset1 = calPulse2Chan(c);
                break;
            case 3: //Hubo-Code equiv -> CalPulse1()
                offset1 = calPulse2Chan(0);
                offset2 = calPulse3Chan(1);
                offset3 = calPulse3Chan(2);
                break;
            default:
                RTT::Logger::log(RTT::Logger::Warning)
                << "Cannot call calibrate on a " << cred->getChannels()
                << " channel controller" << RTT::endlog();
        } 
        return canMsg(credentials->getDevID(), CMD_TXDF, GO_LIMIT_POS, 
                      mode, offset1, offset2, offset3, 0);
    }

    long MotorDevice::calPulse2Chan(int c){
        MotorCredentials* cred = (MotorCredentials*)credentials;
        long offset = 0;
        if(cred->getOffsetPulse(c) >= 0){
            offset = (long)(cred->getDirection(c) *
                            (cred->getOffsetPulse(c) +
                             cred->getEncoderSize(c) * cred->getRevOffset(c)));
        }else{
            offset = (long)(cred->getDirection(c) *
                            (cred->getOffsetPulse(c) -
                             cred->getEncoderSize(c)*cred->getRevOffset(c)));
        }
        offset = canMsg::bitStuffCalibratePacket(offset);
        return offset;
    }

    long MotorDevice::calPulse3Chan(int c){
        MotorCredentials* cred = (MotorCredentials*)credentials;
        if(cred->getOffsetPulse(c) == 128){
            return cred->getDirection(c) * 10 * cred->getRevOffset(c);
        }
        else{
            return 0;
        }
    }
*/
    canMsg MotorDevice::buildHIPpacket(){
        return canMsg( credentials->getDevID(), CMD_TXDF, HIP_ENABLE);
    }

    canMsg MotorDevice::buildRunCmdPacket(){
        return canMsg( credentials->getDevID(), CMD_TXDF, RUN_CMD);
    }

    SensorDevice::SensorDevice(std::string cfg, std::string args)
     :HuboDevice(cfg,args)
    {
        credentials = (ACES::Credentials*)SensorCredentials::makeCredentials(args);
    }

    ACES::Word<float>* SensorDevice::processUS(ACES::Word<canMsg>* msg){
        if(*(msg->getCred()) == *credentials){
            canMsg c = msg->getData();
            switch(c.getType()){
                case SENSOR_FT_RXDF:
                    processUS_SENSOR_FT_RXDF(msg);
                    break;
                case SENSOR_AD_RXDF:
                    processUS_SENSOR_AD_RXDF(msg);
                    break;
                case DAOFFSET_RXDF:
                case ADOFFSET_RXDF:
                case OFFSET_RXDF:
                    //TODO - WTF are these packets for?
                    break;
                case ENC_RXDF:
                case CUR_RXDF:
                case PM_RXDF:
                    //TODO - HuboLab never uses them, I don't see why we would
                    break;
                case NAME_RXDF:
                    processUS_NAME_RXDF(msg);
                    break;
                case STAT_RXDF:
                    processUS_STAT_RXDF(msg);
                    break;
                default:
                    break;
            }
        }
        return NULL;
    }

    void SensorDevice::processUS_SENSOR_FT_RXDF(ACES::Word<canMsg>* msg){
        // For devices which are FT hubolab sensors
        // Channel 0 = Mx, Channel 1 = My, Channel 2 = Fz
        canMsg c = msg->getData();
        SensorCredentials* cred = (SensorCredentials*)credentials;
        float mx = c.getR1() * cred->getScale(0);
        float my = c.getR2() * cred->getScale(1);
        float fz = c.getR3() * cred->getScale(2);
        
        ACES::Word<float> *w = new ACES::Word<float>(mx, 0,
                                  credentials->getDevID(), ACES::REFRESH);
        txUpStream.write(w);

        w = new ACES::Word<float>(my, 1, credentials->getDevID(), ACES::REFRESH);
        txUpStream.write(w);

        w = new ACES::Word<float>(fz, 2, credentials->getDevID(), ACES::REFRESH);
        txUpStream.write(w);
    }

    void SensorDevice::processUS_SENSOR_AD_RXDF(ACES::Word<canMsg>* msg){
        // For devices which are AD hubolab sensors
        // Channel 0 = Acc1, Channel 1 = Acc2, Channel 2 = Gyro1,
        // Channel 3 = Gyro2
        canMsg c = msg->getData();
        SensorCredentials* cred = (SensorCredentials*)credentials;
        float acc1 = c.getR1() * cred->getScale(0);
        float acc2 = c.getR2() * cred->getScale(1);
        float gyro1 = c.getR3() * cred->getScale(2);
        float gyro2 = c.getR4() * cred->getScale(3);
        
        ACES::Word<float> *w = new ACES::Word<float>(acc1, 0,
                                  credentials->getDevID(), ACES::REFRESH);
        txUpStream.write(w);

        w = new ACES::Word<float>(acc2, 1, credentials->getDevID(), ACES::REFRESH);
        txUpStream.write(w);

        w = new ACES::Word<float>(gyro1, 2, credentials->getDevID(), ACES::REFRESH);
        txUpStream.write(w);

        w = new ACES::Word<float>(gyro2, 3, credentials->getDevID(), ACES::REFRESH);
        txUpStream.write(w);
    }

    ACES::Word<canMsg>* SensorDevice::processDS(ACES::Word<float>* w){
        ACES::Word<canMsg> *msg = NULL;
        if( w->getMode() == ACES::REFRESH ){
            canMsg c(0, SEND_SENSOR_TXDF, CMD_NONE);
            msg = new ACES::Word<canMsg>(c);
        }
        //TODO: Clean up the input Word here
        return msg;
    }
}
