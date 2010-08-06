#ifndef ACES_WORD_HPP
#define ACES_WORD_HPP

#include <rtt/Logger.hpp>

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
            ProtoResult(int dev, int node);
            virtual void printme();
            int devID;
            int nodeID;
    };

    template <class T>
    class Result : public  ProtoResult{
        public:
            //Result(T r);
            Result(T r, int dev=0, int node=0);
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
    Result<T>::Result(T r, int dev, int node)
      : ProtoResult(dev, node)
    {
        result = r;
    }
    
    template <class T>
    void Word<T>::printme(){
        RTT::Logger::log() << "Word: " << data << RTT::endlog();
    }
}

#endif 
