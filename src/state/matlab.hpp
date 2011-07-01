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

#include <boost/asio.hpp>

#include "state.hpp"

#ifndef ACES_MATIO_HPP
#define ACES_MATIO_HPP
namespace ACES{
    template <class T>
    class State;

    template <class T>
    class MatlabIO : public RTT::base::RunnableInterface {
        friend class State<T>;
        public:
            //TODO - accessing through a pointer reference like this is a bad
            //idea
            MatlabIO(State<T>* s, unsigned int p);
            bool initialize();
            void step();
            void loop();
            void finalize();
        protected:
            State<T>* parent;
            enum { bufSize = 128 };
            char data[bufSize];
            void scrubBuffer();

            unsigned int port;
            boost::asio::io_service io_service;
            boost::asio::ip::udp::socket socket;
            boost::asio::ip::udp::endpoint sender_endpoint;

            RTT::OutputPort< std::map<std::string, void*>* > DSmatio;
    };

    template <class T>
    MatlabIO<T>::MatlabIO(State<T>* s, unsigned int p)
     : RTT::base::RunnableInterface(),
       parent(s),
       port(p),
       io_service(),
       socket(io_service, 
              boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port))
    {}

    template <class T>
    bool MatlabIO<T>::initialize(){
        return true;
    }

    template <class T>
    void MatlabIO<T>::step(){
    }

    template <class T>
    void MatlabIO<T>::scrubBuffer(){
        for(int i = 0; i < bufSize; i++){
            data[i] = 0;
        }
    }

    template <class T>
    void MatlabIO<T>::loop(){
        while(true){
            socket.receive_from(boost::asio::buffer(data, bufSize),
                                sender_endpoint);
            RTT::Logger::log() << RTT::Logger::Debug << "PACKET: " << data
                               << RTT::endlog();

            std::string ptype;
            std::istringstream line(data);
            T sp;

            line >> ptype;
            if(ptype == "GET"){
                float f = (float)(parent->getVal());
                //float f = 3.1415;
                int len = sprintf(data, "%f", f);
                socket.send_to(boost::asio::buffer(data, len), sender_endpoint);
            }
            if(ptype == "SET"){
                line >> sp;
                T *t = new T(sp);

                std::map<std::string, void*>* n = new std::map<std::string,
                void*>;
                (*n)[parent->getName()] = t;
                DSmatio.write(n);
            }
            if(ptype == "SMP"){
                parent->sample();
            }
            scrubBuffer();
        }
    }

    template <class T>
    void MatlabIO<T>::finalize(){
    }
}

#endif
