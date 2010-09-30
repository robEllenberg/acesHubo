#ifndef ACES_WORD_HPP
#define ACES_WORD_HPP

#include <rtt/Logger.hpp>
#include "credentials.hpp"

namespace ACES{
    class ProtoWord {
        public:
            virtual void printme() = 0;
    };
    
    template <class T>
    class Word : public ProtoWord {
        public:
            Word(T d);
            T data;
            void printme();
    };

    class ProtoResult{
        public:
            ProtoResult(int node, Credentials* c);
            virtual void printme();
            //int devID;
            int nodeID;
            /*! The semiCred is a Credential containing whatever identifying
             *  information the Protocol was able to extract about the device
             *  from the packet */
            Credentials* semiCred;
    };

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
    Word<T>::Word(T d){
        data = d;
    }

    template <class T>
    //Result<T>::Result(T r){
    Result<T>::Result(T r, Credentials* c, int node)
      : ProtoResult(node, c)
    {
        result = r;
    }
    
    template <class T>
    void Word<T>::printme(){
        RTT::Logger::log() << "Word: " << data << RTT::endlog();
    }
}

#endif 
