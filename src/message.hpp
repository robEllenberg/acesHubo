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
            //~Message();
            void printme();
            int size();
            Word<T>* pop();
            Word<T>* peek();
            Word<T>* push(Word<T>* item);
        protected:
            std::deque< Word<T>* > wordList;
    };

    /*
    using namespace std;
    template <class T>
    Message<T>::~Message(){
        while(wordList.size()){
            delete wordList.back;
            wordList.pop_back();
        }
    }
    */

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
    Word<T>* Message<T>::peek(){
        return wordList.front();
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
