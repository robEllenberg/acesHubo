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

#include "omnibot.hpp"

namespace Omnibot{
    PololuDS::PololuDS() : id(0), commandID(0), parameterID(0),
                           parameterVal(0){}

    void PololuDS::printme(){
        RTT::Logger::log() << RTT::Logger::Debug
                           << "ID: " << id
                           << ", Command: " << commandID
                           << ", parameterID: " << parameterID
                           << ", Value: " << parameterVal
                           << RTT::endlog();
    }

    void PololuDS::setID(unsigned char myid){
        id = myid;
    }

    void PololuDS::setCommand(unsigned char command){
        commandID = command;
    }

    void PololuDS::setParameterID(unsigned char pid){
        parameterID = pid;
    }

    void PololuDS::setParameterVal(unsigned char pval){
        parameterVal = pval;
    }

    unsigned char PololuDS::getID(){
        return id;
    }

    unsigned char PololuDS::getCommand(){
        return commandID;
    }

    unsigned char PololuDS::getParameterID(){
        return parameterID;
    }
    
    unsigned char PololuDS::getParameterVal(){
        return parameterVal;
    }

    Protocol::Protocol(std::string cfg, std::string args)
     : ACES::Protocol<unsigned char, PololuDS>(cfg, args){}

    ACES::Message<unsigned char>*
      Protocol::processDS(ACES::Word<PololuDS>* w)
    {
        PololuDS p = w->getData();
        ACES::Message<unsigned char>* m = new ACES::Message<unsigned char>();

        m->push(new ACES::Word<unsigned char>(0xAA) );
        m->push(new ACES::Word<unsigned char>(p.getID()) );
        m->push(new ACES::Word<unsigned char>(p.getCommand()) );

        switch( p.getCommand() ){
            //Fallthroughs are intentional
            case GET_FIRMWARE:
            case GET_ERROR:
            case M0_COAST:
            case M1_COAST:
                break;
            case M0_FWD_SHORT:
            case M0_FWD_LONG:
            case M0_REV_SHORT:
            case M0_REV_LONG:
            case M1_FWD_SHORT:
            case M1_FWD_LONG:
            case M1_REV_SHORT:
            case M1_REV_LONG:
                m->push(new ACES::Word<unsigned char>( p.getParameterVal() ));
                break;
            case GET_CONFIG_PARAM:
                m->push(new ACES::Word<unsigned char>( p.getParameterID() ));
                break;
            case SET_CONFIG_PARAM:
                m->push(new ACES::Word<unsigned char>( p.getParameterID() ));
                m->push(new ACES::Word<unsigned char>( p.getParameterVal() ));
                m->push(new ACES::Word<unsigned char>( 0x55 ));
                m->push(new ACES::Word<unsigned char>( 0x2A ));
                break;
            default:
                delete m;
                return NULL;
        }
        return m;
    }

    Device::Device(std::string cfg, std::string args)
     : ACES::Device<float, PololuDS>(cfg){
        credentials = ACES::Credentials::makeCredentials(args);
        this->addOperation("setID", &Device::setID, this,
                    RTT::OwnThread).doc("Assign a new Device ID");
     }

    void Device::setID(int newID){
        PololuDS p;
        p.setID( credentials->getDevID() );
        p.setCommand( SET_CONFIG_PARAM );
        p.setParameterID( PARAM_DEVID );
        p.setParameterVal( newID );
        txDownStream.write( new ACES::Word<PololuDS>(p) );

        //Create a new credential with the reassigned ID
        credentials = ACES::Credentials::makeCredentials((int) newID);
    }

    ACES::Word<PololuDS>* Device::processDS(ACES::Word<float>* w){
        if(w){
            PololuDS p;
            p.setID(credentials->getDevID());
            switch( w->getMode() ){
                case(ACES::REFRESH):
                    break;
                case(ACES::SET):
                    buildMotionCmdPacket(p, w->getNodeID(), (int)w->getData());
                    break;
            }
            ACES::Word<PololuDS> *pw = new ACES::Word<PololuDS>(p);
            return pw;
        }
        return NULL;
    }

    void Device::buildMotionCmdPacket(PololuDS &p, unsigned char nodeID,
                                      int data)
    {
        switch( nodeID ){
            case 0:
                if( data < 0 ){
                    p.setCommand(M0_REV_SHORT);
                    data *= -1;
                }
                else{
                    p.setCommand(M0_FWD_SHORT);
                }
                break;

            case 1:
                if( data < 0 ){
                    p.setCommand(M1_REV_SHORT);
                    data *= -1;
                }
                else{
                    p.setCommand(M1_FWD_SHORT);
                }
                break;
        }
        unsigned char d = 0;
        if(data > 127)
            data = 127;
        d = data;
        p.setParameterVal( d );
    }

};
