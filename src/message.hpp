#ifndef ACES_MESSAGE_HPP
#define ACES_MESSAGE_HPP

#include <deque>
#include <rtt/Logger.hpp>
//#include "credentials.hpp"
#include "word.hpp"

namespace ACES{
    template <class T>
    class Message {
        public:
            void printme();
            int size();
            Word<T>* pop();
            Word<T>* push(Word<T>* item);
        protected:
            std::deque< Word<T>* > wordList;
    };

    template <class T>
    int Message<T>::size(){
        return wordList.size();
    }

    template <class T>
    Word<T>* Message<T>::pop(){
        Word<T>* item = wordList.front();
        wordList.pop_front();
        return item;
    }

    template <class T>
    Word<T>* Message<T>::push(Word<T>* item){
        wordList.push_back(item);
        return item;
    }

    template <class T>
    void Message<T>::printme(){
        typename std::deque< Word<T>* >::iterator it;
        for(it = wordList.begin(); it != wordList.end(); it++){
                //RTT::Logger::log() << "\t";
                (*it)->printme();
        }
    }
    
}    
#endif
