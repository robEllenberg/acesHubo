#include "word.hpp"

namespace ACES{
    ProtoWord::ProtoWord(int nID, int dID, int m, Credentials* c){
        nodeID = nID;
        devID = dID;
        cred = c;
        mode = m;
    }

    ProtoWord::ProtoWord(ProtoWord& w){
        nodeID = w.getNodeID();
        devID = w.getDevID();
        cred = w.getCred();
        mode = w.getMode();
    }

    int ProtoWord::getNodeID(){
        return nodeID;
    }

    int ProtoWord::getDevID(){
        return devID;
    }

    Credentials* ProtoWord::getCred(){
        return cred;
    }

    bool ProtoWord::setCred(Credentials* c){
        //TODO - Add some kind of typecheck here
        if(c != NULL){
            cred = c;
        }
        return true;
    }

    int ProtoWord::getMode(){
        return mode;
    }
    
}
