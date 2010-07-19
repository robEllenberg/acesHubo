#ifndef ACES_WORD_HPP
#define ACES_WORD_HPP

namespace ACES{
    class ProtoWord {
    };
    
    template <class T>
    class Word : public ProtoWord {
        public:
            Word(T d);
            T data;
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
}

#endif 
