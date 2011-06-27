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
            offsetPulse[i] = 0;
            revOffset[i] = 0;
            CCW[i] = true;
            harmonic[i] = 0;
        }
    }

    ACES::Credentials* MotorCredentials::makeCredentials(std::string args){
        std::istringstream s(args);
        int board, channels;
        s >> board >> channels;
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
        RTT::Logger::log() << "]\nOffset Pulse: [";
        for(int i = 0; i < ctrlSize; i++){
            RTT::Logger::log() << offsetPulse[i] << ", ";
        }
        RTT::Logger::log() << "]\nRev Offset: [";
        for(int i = 0; i < ctrlSize; i++){
            RTT::Logger::log() << revOffset[i] << ", ";
        }
        RTT::Logger::log() << "]\nCCW?: [";
        for(int i = 0; i < ctrlSize; i++){
            RTT::Logger::log() << CCW[i] << ", ";
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
        s >> id >> channels;
        return (ACES::Credentials*)new SensorCredentials(di, channels);
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

    float SensorCredentials::getScale(int chan);
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

    bool setDirection(int chan, float dir){
        if(checkChannel(chan)){
            direction[chan] = dir;
            return true;
        }
        return false;
    }

    bool setScale(int chan, float sca){
        if(checkChannel(chan)){
            scale[chan] = sca;
            return true;
        }
        return false;
    }

    #define TESTMODE 1
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
                               .arg("b0123", "Lower Order Bytes - Hex String")
                               .arg("b4567", "Upper Order Bytes - Hex String");
    }

    bool CANHardware::startHook(){
        channel = open(fd.c_str(), O_RDWR | O_NONBLOCK | O_CREAT | O_TRUNC,
                       0755);

        #if TESTMODE == 1
            // Perform no special commands    
        #else //Normal operation action
            //Begin - snippit from can4linux examples baud.c
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
    }

    bool CANHardware::genPacket(int ID, int len, 
                                std::string lower, std::string upper)
    {
        ACES::Message<canmsg_t*> *m = new ACES::Message<canmsg_t*>();
        unsigned int b0123 = 0, b4567 = 0;
        std::istringstream u(upper), l(lower);
        l >> std::setbase(16) >> b0123;
        u >> std::setbase(16) >> b4567;

        canmsg_t* msg = new canmsg_t;
        msg->id = ID;
        msg->cob = 0;
        msg->flags = 0;
        msg->length = len;
        switch(len){
            case 8:
                msg->data[7] = ((b4567 & 0x000000FFu) >> (8*0));
            case 7:
                msg->data[6] = ((b4567 & 0x0000FF00u) >> (8*1));
            case 6:
                msg->data[5] = ((b4567 & 0x00FF0000u) >> (8*2));
            case 5:
                msg->data[4] = ((b4567 & 0xFF000000u) >> (8*3));
            case 4:
                msg->data[3] = ((b0123 & 0x000000FFu) >> (8*0));
            case 3:
                msg->data[2] = ((b0123 & 0x0000FF00u) >> (8*1));
            case 2:
                msg->data[1] = ((b0123 & 0x00FF0000u) >> (8*2));
            case 1:
                msg->data[0] = ((b0123 & 0xFF000000u) >> (8*3));
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
                dprintf(channel, "%2.2X\t%d\t\t", (int)(msg->id), msg->length);
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
            delete msg;
            //TODO - Delete the word
        }
        delete m;
        return true;
    }

    canmsg_t rxBuffer[canBuffSize]; 

    void CANHardware::rxBus(int size){
        Word<canmsg_t*>* w = NULL;
        float time = 0;
        int channel = 0, len = 0, rxSize = 0;
        unsigned long msgID = 0;
        std::string junk;

        #if TESTMODE == 1 //Offline operation
            while( getline() and (rxSize < canBuffSize)){
                rxSize++;
                s >> time >> channel >> msgID >> junk >> junk >> len;
                rxBuffer[n].id = msgID;
                rxBuffer[n].length = len;
                //switch stream to hex mode
                for(int i = 0, temp = 0; i < len; i++){
                    s >> temp;
                    rxBuffer[n].data[i] = temp;
                }
            }
        #else             //Online operation
            rxSize = read(channel, &rxBuffer, canBufSize);
        #endif

        for(int i = 0; i < rxSize; i++){
            canmsg_t* msg = new canmsg_t;
            msg->flags = rxBuffer[i].flags;
            msg->cob = rxBuffer[i].cob;
            msg->id = rxBuffer[i].id;
            msg->timestamp = rxBuffer[i].timestamp;
            msg->length = rxBuffer[i].length;
            for(int j = 0; j < length; j++){
                msg->data[j] = rxBuffer[i].data[j];
            }

            Word<canmsg_t*>* w = new Word<canmsg_t*>(msg);
            usQueue.enqueue(w);
        }
    }

    Protocol::Protocol(std::string cfg, std::string args)
      : ACES::Protocol<canmsg_t*, canMsg>(cfg, args)
    {}

    ACES::Message<canmsg_t*>* Protocol::processDS(ACES::Word<canMsg>* h){
        ACES::Message<canmsg_t*> *m = NULL;
        if(h){
            canmsg_t* msg = h->getData().toLineType();
            m = new ACES::Message<canmsg_t*>;
            m->push( new ACES::Word<canmsg_t*>(msg) );
        }
        return m;
    }

    ACES::Word<canMsg>* Protocol<canmsg_t*, canMsg>::processUS(ACES::Word<canmsg_t*>* usIn){
        ACES::Word<canMsg>* pw = NULL;
        canmsg_t* c = usIn->getData();
        unsigned long r1 = 0, r2 = 0, r3 = 0, r4 = 0, r5 = 0;
        huboCanType idClass = CAN_NONE;

        if( offsetRange(SENSOR_FT_RXDF, c->id)){
            idClass = SENSOR_FT_RXDF;
            r1 = assemble2Byte(c->data[0], c->data[1]); //Mx
            r2 = assemble2Byte(c->data[2], c->data[3]); //My
            r3 = assemble2Byte(c->data[4], c->data[5]); //Fz
        }
        else if( offsetRange(SENSOR_AD_RXDF, c->id)){
            idClass = SENSOR_AD_RXDF;
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

        int id = c->id % (int)idClass;
        canMsg msg(id, idClass, CMD_NONE, r1, r2, r3, r4, r5);
        pw = new Word<canMsg>(msg);
        return pw;
    }

    /*! \param t Indicator of the range we wish to test against
        \param lineID The message ID used in the CAN packet

        Function is only useful when comparing the gyro/adc packets
    */
    bool Protocol::offsetRange(huboCanType t, int lineID){
        int baseval = (int)t;
        //All address ranges for these types of sensors are 0x10 wide
        if( (lineID >= baseval) or (lineID < baseval+0x10) ){
            return true;
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

        this->addOperation("setOffsetPulse", &MotorDevice::setOffsetPulse, this,
            RTT::OwnThread).doc("Set the gains for this controller")
                           .arg("channel", "Channel number to set")
                           .arg("offset", "The offset in encoder ticks");

        this->addOperation("setRevOffset", &MotorDevice::setRevOffset, this,
            RTT::OwnThread).doc("Set the gains for this controller")
                           .arg("channel", "Channel number to set")
                           .arg("offset", "The offset in encoder ticks");

        this->addOperation("setCCW", &MotorDevice::setCCW, this,
            RTT::OwnThread).doc("Set the direction of rotation for calibration")
                           .arg("channel", "Channel number to set")
                           .arg("ccw", "Direction: true=ccw, false=cw");

        this->addOperation("setHarmonic", &MotorDevice::setHarmonic, this,
            RTT::OwnThread).doc("Set the harmonic drive ratio.")
                           .arg("channel", "Channel number to set")
                           .arg("harmonic", "Drive ratio");

        this->addOperation("calibrate", &MotorDevice::setCalibrate, this,
            RTT::OwnThread).doc("Send a calibreation pulse to the controller"
                                " (zero the motor)")
                           .arg("channel", "Channel number to send pulse on");

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
        RTT::Logger::log(RTT::Logger::Warning) <<  "Processing: " << msg
        << RTT::endlog();
        return msg;
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

    bool MotorDevice::setOffsetPulse(int chan, int offset){
        return ((MotorCredentials*)credentials)->setOffsetPulse(chan, offset);
    }

    bool MotorDevice::setRevOffset(int chan, int offset){
        return ((MotorCredentials*)credentials)->setRevOffset(chan, offset);
    }

    bool MotorDevice::setCCW(int chan, bool CCW){
        return ((MotorCredentials*)credentials)->setCCW(chan, CCW);
    }

    bool MotorDevice::setHarmonic(int chan, int harmonic){
        return ((MotorCredentials*)credentials)->setHarmonic(chan, harmonic);
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
                    RTT::Logger::log(RTT::Logger::Warning) <<  "Invalid channel number "
                        << channel << " specified. Must be [0-" << channel-1 << "]."
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
                    RTT::Logger::log(RTT::Logger::Warning) <<  "Invalid channel number "
                        << channel << " specified. Must be [0-" << channel-1 << "]."
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

    bool MotorDevice::setCalibrate(int channel){
        if( ((MotorCredentials*)credentials)->checkChannel(channel) ){
            canMsg c = buildCalibratePulse(channel);
            ACES::Word<canMsg>* msg = buildWord(c, channel);
            txDownStream.write(msg);
            return true;
        }
        return false;
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
                        canMsg::bitStuff15byte((long)(setPoint[i]*dir*ppr/360.));
                }
                break;
            case 3: //Three Channel Motor Controllers - Wrists & Neck
                for(int i = 0; i < 3; i++){
                    dir = ((MotorCredentials*)credentials)->getDirection(i);
                    ppr = ((MotorCredentials*)credentials)->getHarmonic(i)
                          *((MotorCredentials*)credentials)->getGearRatio(i)
                          *((MotorCredentials*)credentials)->getEncoderSize(i);
                    temp[i] = setPoint[i]*dir*ppr/360.;
                }
                break;
            case 2: //Two Channel Motor Controllers - Limbs
                for(int i = 0; i < 2; i++){
                    dir = ((MotorCredentials*)credentials)->getDirection(i);
                    ppr = ((MotorCredentials*)credentials)->getHarmonic(i)
                          *((MotorCredentials*)credentials)->getGearRatio(i)
                          *((MotorCredentials*)credentials)->getEncoderSize(i);
                    temp[i] =
                       canMsg::bitStuff3byte((long)(setPoint[i]*dir*ppr/360.));
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

    SensorDevice::SensorDevice(std::string cfg, std::string args)
     :HuboDevice(cfg,args)
    {
        credentials = ACES::Credentials::makeCredentials(args);
    }

    Word<float>* SensorDevice::processUS(Word<canMsg>* w){
        Word<float>* out;
        msg = pw->getData();
        switch(msg.type){
            case SENSOR_FT_RXDF:
                float val = XX?
                id = 
            case SENSOR_AD_RXDF:
            case DAOFFSET_RXDF:
            case ADOFFSET_RXDF:
            case OFFSET_RXDF:
        }
        txUpStream.write(float_word);
        return NULL;
    }
    /*
    canMsg MotorDevice::buildRefreshPacket(){
        canMsg cm(0, SEND_SENSOR_TXDF, (cmdType)0);
        return cm;
    }
    */

    /*
    ACES::Word<canMsg>* SensorDevice::processDS(ACES::Word<float>* w){
        if( w->getMode() == ACES::REFRESH ){
            canMsg msg();
        }
    }
    */
}
