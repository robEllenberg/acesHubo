#include "device.hpp"

namespace ACES{
    template <class S, class PD>
    Device<S,PD>::Device(std::string config) :
      taskCfg(config),
      RTT::TaskContext(name)
      //txDownStream("txDownStream"),
      //txUpStream("txUpStream"),
      //dsQueue(10),
      //usQueue(10),
    {
        /*this->events()->addEvent(&txDownStream, "txDownStream", "goal",
                                 "The Goal/SP Data");
        this->events()->addEvent(&txUpStream, "txUpStream", "data",
                                 "Interperted data going to states");*/
        this->addOperation("credentials", &Device<S,PD>::printCred, this,
                           RTT::OwnThread).doc("Print the Credentials");
        this->ports()->addPort("RxDS", rxDownStream).doc(
                               "DownStream (from State) Reception");
        this->ports()->addPort("RxUS", rxUpStream).doc(
                               "UpStream (from Protocol) Reception");
        this->ports()->addPort("TxDS", txDownStream).doc(
                               "DownStream (to Protocol) Transmission");
        this->ports()->addPort("TxUS", txUpStream).doc(
                               "UpStream (to State) Transmission");
        this->setActivity(
            new RTT::Activity( priority, 1.0/freq, 0, name )
        );
    }

/*
    template <class S, class PD>
    void Device<S,PD>::rxDownStream(Word<S>* g){
        bool i = g->setCred(credentials);
        assert(i);
        dsQueue.enqueue(g);
    }

    template <class S, class PD>
    void Device<S,PD>::rxUpStream(Word<PD>* rx){
        //First we check if the two device types are the same
        if(*(rx->getCred()) == *credentials){
            RTT::Logger::log() << RTT::Logger::Debug
                               << "(dev) Cred Match" << RTT::endlog();
            //If they are, we process them
            usQueue.enqueue(rx);
        }
    }
*/
    template <class S, class PD>
    bool Device<S,PD>::subscribeState(RTT::TaskContext* s){
        this->connectPeers( (RTT::TaskContext*) s);
        //TODO - Figure out how calling this reception function
        //in a non-periodic thread affects RT characteristic

        RTT::base::PortInterface* port = NULL;
        bool success;
        RTT::ConnPolicy policy = RTT::ConnPolicy::buffer(10);

        port = (RTT::base::PortInterface*)s->ports()->getPort("RxUS");
        success = this->txUpStream.connectTo(port, policy);
        if(not success){
            return false;
        }
        /*RTT::Handle h = s->events()->setupConnection("txDownStream")
            .callback( this, &Device<S,PD>::rxDownStream
                     ,  s->engine()->events()
                     ).handle();*/

        port = (RTT::base::PortInterface*)s->ports()->getPort("TxDS");
        success = port->connectTo(this->rxDownStream, policy);
        if(not success){
            return false;
        }
        /*h = this->events()->setupConnection("txUpStream")
            .callback( s, &State<S>::rxUpStream
          //           ,  this->engine()->events()
                     ).handle();*/
        return true;
    }
}
