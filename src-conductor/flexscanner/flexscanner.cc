/* 
    Conductor: High Degree of Freedom Robot Controller Framework
    Copyright (C) 2010, 2011 Robert Sherbert
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
    please contact the author. The software can be licensed to you under
    non-free license terms by the copyright holder.

    As a courtesy to the author, and in the spirit of fair attribution, you are
    asked to cite the following paper if any work based upon or utilizing this
    framework is published in the scientific literature: 
    Sherbert, Robert M. and Oh, Paul Y. "Conductor: A Controller Development
    Framework for High Degree of Freedom Systems." Intelligent Robots and
    Systems (IROS), 2011 IEEE/RSJ International Conference on. 
*/

namespace FlexScanner{
    Hardware::Hardware(std::string cfg, std::string args)
     : ACES::Hardware<unsigned char>(cfg, args),
        io_service(),
        port(io_service, (const char*)args.c_str()),
        rxReader(&port),
        rxActivity(priority, &rxReader)
    {
        RTT::ConnPolicy policy = RTT::ConnPolicy::buffer(200);
        rxReader.out.connectTo(&rxBuf, policy);
    }
    
    bool Hardware::startHook(){
        rxActivity.start();
        return true;
    }

    void Hardware::stopHook(){
        rxActivity.stop();
    }

    void Hardware::rxBus(int size){
        unsigned char c;
        while(rxBuf.read(c) == RTT::NewData){
            RTT::Logger::log() << "Got Something! - " << (int)c
                               << RTT::endlog();
            txUpStream.write(new ACES::Word<unsigned char>(c));
        }
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

    template <class USPacket, class FlexScanner>
    Protocol<USPacket, FlexScanner>::Protocol(std::string cfg, std::string args)
     : ACES::Protocol<unsigned char, USPacket>(cfg, args)
    {
        scanner = NULL;
    }

    template <class USPacket, class FlexScanner>
    bool Protocol<USPacket, FlexScanner>::startHook(){
        scanner = new FlexScanner();
        return true;
    }

    template <class USPacket, class FlexScanner>
    void Protocol<USPacket, FlexScanner>::stopHook(){
        delete scanner;
        scanner = NULL;
    }

    template <class USPacket, class FlexScanner>
    ACES::Word<USPacket>*
      Protocol<USPacket, FlexScanner>::processUS(ACES::Word<unsigned char>* usIn)
    {
        ACES::Word<USPacket>* pw = NULL;
        unsigned char c = 0;
        c = usIn->getData();
        //RTT::Logger::log() << "Got Something! - " << (int)c << RTT::endlog();

        std::string temp(1, c);
        istringstream *in = new istringstream(temp);
        
        //If yylex returns 1 we have matched a full packet
        //if return is 0 we have only eaten a character
        if( scanner->yylex((istream*)in) ){ 
            USPacket* p = scanner->releasePacket();

            ACES::Word<USPacket>* word= new ACES::Word<USPacket>(*p, 0, 0, 0,
                (ACES::Credentials*)credFromPacket(p));
            p->printme();
            delete in;
            delete p;
            return word;
        }
        return NULL;
    }
}
