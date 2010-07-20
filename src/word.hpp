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
    };

    template <class T>
    class Result : public  ProtoResult{
        public:
            Result(T r);
            int id;
            int busid;
            T result;
    };

    template <class T>
    Word<T>::Word(T d){
        data = d;
    }

    template <class T>
    Result<T>::Result(T r){
        result = r;
    }
    
    template <class T>
    void Word<T>::printme(){
        RTT::Logger::log() << "Word: " << data << RTT::endlog();
    }
}

#endif 
