#include "word.hpp"

namespace ACES{
    template <class T>
    int Word<T>::getNodeID(){
        return nodeID;
    }

    template <class T>
    int Word<T>::getDevID(){
        return devID;
    }

    template <class T>
    Credentials* Word<T>::getCred(){
        return cred;
    }

    template <class T>
    int Word<T>::getMode(){
        return mode;
    }
}
