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
    Credentials::Credentials(int board, int chan) : ACES::Credentials(board)
    {
        if(chan > 0 && chan <=3){
            channels = chan;
        } else{
            RTT::Logger::log(RTT::Logger::Warning) <<  "Invalid channel number "
                << chan << " specified. Must be [1-3]." << RTT::endlog();
        }
    }

    Credentials* Credentials::makeCredentials(std::string args){
        std::istringstream s(args);
        int board, channels;
        s >> board >> channels;
        return new Credentials(board, channels);
    }

    int Credentials::getChannels(){
        return channels;
    }

    void Credentials::setPPR(int chan, float proposedPPR){
        PPR[chan] = proposedPPR;
    }

    void Credentials::setDirection(int chan, float dir){
        direction[chan] = dir;
    }

    float Credentials::getPPR(int chan){
        if((chan < channels) and (chan >= 0)){
            return PPR[chan];
        }
        return 0.0;
    }

    float Credentials::getDirection(int chan){
        if((chan < channels) and (chan >= 0)){
            return direction[chan];
        }
        return 0.0;
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
        channel = open(fd.c_str(), O_RDWR | O_NONBLOCK);

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
            #if TESTMODE == 1
                void* p = &(msg->data);
                int r = write(channel, p, msg->length);
            #else //Normal operation
                int r = write(channel, msg, 1);
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

    void CANHardware::rxBus(int size){
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

    MotorDevice::MotorDevice(std::string cfg, std::string args)
      : ACES::Device<float, canMsg>(cfg), instantTrigger(false)
    {
        credentials =
            (ACES::Credentials*)Credentials::makeCredentials(args);

        this->addOperation("setTicks", &MotorDevice::setPPR, this,
            RTT::OwnThread).doc("Set the PPR (Pulses/Revolution) of a channel")
                           .arg("channel", "Channel number to set")
                           .arg("PPR", "Number of ticks per revolution");

        this->addOperation("setDirection", &MotorDevice::setPPR, this,
            RTT::OwnThread).doc("Set the direction of rotation for a channel")
                           .arg("channel", "Channel number to set")
                           .arg("dir", "Direction of rotation = +/-1");

        this->addAttribute("instantTrigger", instantTrigger);
    }

    ACES::Word<canMsg>* MotorDevice::processDS(ACES::Word<float>* w){
        ACES::Word<canMsg>* msg = NULL;
        if(w and (w->getMode() == ACES::SET) ){
            msg = setSetPoint(w->getNodeID(), w->getData(), instantTrigger);
        }
        return msg;
    }

    int MotorDevice::getChannels(){
        return ((Credentials*)credentials)->getChannels();
    }

    bool MotorDevice::setPPR(int chan, float proposedPPR){
        int numChan = getChannels();
        if(chan > 0 && chan <= numChan){
            ((Credentials*)credentials)->setPPR(chan, proposedPPR);
            return true;
        }
        else{
            RTT::Logger::log(RTT::Logger::Warning) <<  "Invalid channel number "
                << chan << " specified. Must be [1-" << numChan << "]."
                << RTT::endlog();
        }
        return false;
    }

    bool MotorDevice::setDirection(int chan, float dir){
        int numChan = getChannels();
        if(chan > 0 && chan <= numChan){
            ((Credentials*)credentials)->setDirection(chan, dir);
            return true;
        }
        else{
            RTT::Logger::log(RTT::Logger::Warning) <<  "Invalid channel number "
                << chan << " specified. Must be [1-" << numChan << "]."
                << RTT::endlog();
        }
        return false;
    }

    ACES::Word<canMsg>* MotorDevice::setSetPoint(int channel,
                                                 float sp,
                                                 bool instantTrigger)
    {
        ACES::Word<canMsg>* w = NULL;
        int numChan = getChannels();
        if(channel > 0 && channel <= numChan){
            setPoint[channel] = sp;
            trigger[channel] = true;
            
            if(instantTrigger or triggersSet()){
                Credentials* cred = (Credentials*)credentials;
                new ACES::Word<canMsg>(buildSetPacket(), channel, cred->getDevID(),
                                 ACES::SET, cred);
                clearTrigger();
            }
        }
        else{
            RTT::Logger::log(RTT::Logger::Warning) <<  "Invalid channel number "
                << channel << " specified. Must be [1-" << numChan << "]."
                << RTT::endlog();
        } 
        return w;
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

    canMsg MotorDevice::buildSetPacket(){
        int numChan = getChannels(); 
        unsigned long temp[5];
        switch(numChan){
            float dir, ppr;
            case 5: //Five Channel Motor Controller - Fingers
                for(int i=0; i < 5; i++){
                    dir = ((Credentials*)credentials)->getDirection(i);
                    ppr = ((Credentials*)credentials)->getPPR(i);
                    temp[i] =
                        canMsg::bitStuff15byte((long)setPoint[i]*dir*ppr/360.);
                }
                break;
            case 3: //Three Channel Motor Controllers - Wrists & Neck
                for(int i = 0; i < 3; i++){
                    dir = ((Credentials*)credentials)->getDirection(i);
                    ppr = ((Credentials*)credentials)->getPPR(i);
                    temp[i] = setPoint[i]*dir*ppr/360.;
                }
                break;
            case 2: //Two Channel Motor Controllers - Limbs
                for(int i = 0; i < 2; i++){
                    dir = ((Credentials*)credentials)->getDirection(i);
                    ppr = ((Credentials*)credentials)->getPPR(i);
                    temp[i] =
                       canMsg::bitStuff3byte((long)setPoint[i]*dir*ppr/360.);
                }
                break;
            default:
                //TODO - Failure case
                break;
        }

        canMsg cm( credentials->getDevID(),
                CMD_TXDF, (cmdType)numChan, temp[0], temp[1], temp[2], temp[3],
                temp[4]);
        return cm;
    }

    canMsg MotorDevice::buildRefreshPacket(){
        canMsg cm(0, SEND_SENSOR_TXDF, (cmdType)0);
        return cm;
    }

    /*
    ACES::Word<canMsg>* SensorDevice::processDS(ACES::Word<float>* w){
        if( w->getMode() == ACES::REFRESH ){
            canMsg msg();
        }
    }
    */
}
