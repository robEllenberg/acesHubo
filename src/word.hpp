#ifndef ACES_WORD_HPP
#define ACES_WORD_HPP

#include <rtt/Logger.hpp>
#include "credentials.hpp"

namespace ACES{
    enum MODES { REFRESH=1, SET};

    template <class T>
    class HWord {
        public:
            HWord();
            HWord(T d);
            HWord(PDWord*);
            virtual void printme();
            T getData();
        protected:
            T data;
    };

    template <class T>
    class PDWord : HWord<T> {
        public:
            PDWord();
            PDWord(T d, int nID=0, int dID=0, Credentials* c=NULL);
            virtual void printme() = 0;

            int getNodeID();
            int getDevID();
            Credentials* getCred();
            /*! The semiCred is a Credential containing whatever identifying
             *  information the Protocol was able to extract about the device
             *  from the packet */
        protected:
            int nodeID; //!Identify the type of node on the Device
            int devID;
            Credentials* cred;
    };

    template <class T>
    class SWord : PDWord<T> {
        public:
            SWord();
            SWord(T d, int nID=0, int dID=0, Credentials* c=NULL, int m=0);
            virtual void printme();
            int getMode();
        protected:
            int mode; //!The objective of this Goal packet (Refresh, Set, etc)
    };

    template <class T>
    HWord<T>::HWord(T d){
        data = d;
    }

    template <class T>
    HWord<T>::HWord(){
        data = 0;
    }

    template <class T>
    HWord<T>::HWord(PDWord* p){
        data = p->getData();
    }

    template <class T>
    void HWord<T>::printme(){
        RTT::Logger::log() << "HWord: " << data << RTT::endlog();
    }

    template <class T>
    T HWord<T>::getData(){
        return data;
    }

    template <class T>
    PDWord<T>::PDWord(T d, int nID, int dID, Credentials* c) : HWord<T>(d){
        nodeID = nID;
        devID = dID;
        cred = c;
    }

    template <class T>
    PDWord<T>::PDWord() : HWord<T>() {
        nodeID = 0;
        devID = 0;
        cred = 0;
    }

    template <class T>
    void PDWord<T>::printme(){
        RTT::Logger::log() << "PDWord N(" << nodeID << "), "
                              "D(" << devID << "): "
                           << this->data << RTT::endlog();
        cred->printme();
    }

    template <class T>
    SWord<T>::SWord() : PDWord<T>() {
        mode = 0;
    }
    
    template <class T>
    SWord<T>::SWord(T d, int nID, int dID, Credentials* c, int m):
      PDWord<T>(d, nID, dID, c)
    {
        mode = m;
    }

    template <class T>
    void SWord<T>::printme(){
        RTT::Logger::log() << "SWord N(" << this->nodeID << "), "
                              "D(" << this->devID << "), "
                              "M(" << this->mode << "): "
                           << this->data << RTT::endlog();
        this->cred->printme();
    }
/*
    template <class T>
    class Result : public  ProtoResult{
        public:
            //Result(T r);
            Result(T r, Credentials* c, int node=0);
            //int id;
            //int busid;
            T result;
    };

    template <class T>
    //Result<T>::Result(T r){
    Result<T>::Result(T r, Credentials* c, int node)
      : ProtoResult(node, c)
    {
        result = r;
    }
*/
}

#endif 
