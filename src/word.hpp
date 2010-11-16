#ifndef ACES_WORD_HPP
#define ACES_WORD_HPP

#include <rtt/Logger.hpp>
#include "credentials.hpp"

namespace ACES{
    enum MODES { REFRESH=1, SET};

    class ProtoWord {
        public:
            ProtoWord();
            ProtoWord(ProtoWord& w);
            ProtoWord(int nID=0, int dID=0, int m=0, Credentials* c=NULL);

            int getNodeID();
            int getDevID();
            int getMode();
            Credentials* getCred();
            bool setCred(Credentials* c);
        protected:
            int nodeID; //!Identify the type of node on the Device
            int devID;
            Credentials* cred;
            int mode; //!The objective of this Goal packet (Refresh, Set, etc)
    };

    template <class T>
    class Word : public ProtoWord {
        public:
            Word();
            Word( Word& w);
            Word(T d, Word& w);
            Word(T d, int nID=0, int dID=0, int m=0, Credentials* c=NULL);

            void printme();
            T getData();
        protected:
            T data;
    };

    template <class T>
    Word<T>::Word(T d, int nID, int dID, int m, Credentials* c)
     : ProtoWord(nID, dID, m, c){
        data = d;
    }

    template <class T>
    Word<T>::Word( Word &w) : ProtoWord(static_cast<ProtoWord&>(w) ) {
        data = w.getData();
    }

    template <class T>
    Word<T>::Word(T d, Word& w) : ProtoWord(static_cast<ProtoWord&>(w) ) {
        data = d;
    }

    template <class T>
    T Word<T>::getData(){
        return data;
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
        RTT::Logger::log() << "= " << (int)this->data << RTT::endlog();

        if(cred){
            cred->printme();
        }
    }
}

#endif 
