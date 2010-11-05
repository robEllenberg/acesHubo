#ifndef ACES_WORD_HPP
#define ACES_WORD_HPP

#include <rtt/Logger.hpp>
#include "credentials.hpp"

namespace ACES{
    enum MODES { REFRESH=1, SET};

    template <class T>
    class Word : Word<T> {
        public:
            Word();
            Word(Word* w);
            Word(T d, int nID=0, int dID=0, int m=0, Credentials* c=NULL);

            virtual void printme();
            T getData();
            int getNodeID();
            int getDevID();
            int getMode();
            Credentials* getCred();
        protected:
            T data;
            int nodeID; //!Identify the type of node on the Device
            int devID;
            Credentials* cred;
            int mode; //!The objective of this Goal packet (Refresh, Set, etc)
    };

    template <class T>
    Word<T>::Word(T d, int nID, int dID, int m, Credentials* c){
        data = d;
        nodeID = nID;
        devID = dID;
        cred = c;
        mode = m;
    }

    template <class T>
    Word<T>::Word(const Word& w){
        data = w.getData();
        nodeID = w.getNodeID();
        devID = w.getDevID();
        cred = w.getCred();
        mode = w.getMode();
    }

    template <class T>
    T HWord<T>::getData(){
        return data;
    }

    template <class T>
    void PDWord<T>::printme(){
        RTT::Logger::log() << "PDWord N(" << nodeID << "), "
                              "D(" << devID << "): "
                           << this->data << RTT::endlog();
        cred->printme();
    }

    template <class T>
    void Word<T>::printme(){
        RTT::Logger::log() << "Word ";
        if(nodeID){
            RTT::Logger::log() << "N(" << this->nodeID << "), ";
        }
        if(devID){
            RTT::Logger::log() << "D(" << this->devID << "), ";
        }
        if(mode){
            RTT::Logger::log() << "M(" << this->mode << "): ";
        }
        RTT::Logger::log() << "= " << this->data << RTT::endlog();

        if(cred){
            cred->printme();
        }
    }
}

#endif 
