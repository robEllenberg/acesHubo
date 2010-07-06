#include "dispatcher.hpp"

namespace ACES{
    Dispatcher::Dispatcher(std::string n, int pri,
                           int UpdateFreq)
                           :RTT::TaskContext(n)
    {
        name = n;
        frequency = UpdateFreq;
        priority = pri;
        this->setActivity(
             new RTT::Activity( priority, 1.0/frequency, 0, n ) );
    }

    bool Dispatcher::startHook(){
        std::map<Parameter*, Protocol*>::iterator it;
        for(it = this->routeTable.begin();
            it != this->routeTable.end();
            it++)
        {
            ((*it).second)->start();
        }

        for(it = this->routeTable.begin();
            it != this->routeTable.end();
            it++)
        {
            ((*it).first)->start();
        }
        return true;
    }
    bool Dispatcher::configureHook(){
        return true;
    }
    void Dispatcher::updateHook(){
        //RTT::Logger::log() << "Update Dispatcher"
        //                   << RTT::Logger::endl;
    }

    void Dispatcher::stopHook(){
        std::map<Parameter*, Protocol*>::iterator it;

        for(it = this->routeTable.begin();
            it != this->routeTable.end();
            it++)
        {
            ((*it).first)->stop();
        }

        for(it = this->routeTable.begin();
            it != this->routeTable.end();
            it++)
        {
            ((*it).second)->stop();
        }
    }    

    void Dispatcher::cleanupHook(){
    }
            
    bool Dispatcher::registerParameter(Parameter* pram,
                                       Protocol* pcol)
    {
        routeTable[pram] = pcol;
        //Create a bi-directional data-flow
        //between the parameter & the protocol
        (*pram).connectPeers( pcol );

        std::string n1 = (std::string)"Update Request(TX): "
                       + (*pram).name;
        std::string n2 = (std::string)"Update Request(RX): "
                       + (*pram).name;
        std::string n3 = (std::string)"Update Response(TX): "
                       + (*pram).name;
        std::string n4 = (std::string)"Update Response(RX): "
                       + (*pram).name;

        RTT::WriteDataPort<Credentials*>* pcol_tx =
            new RTT::WriteDataPort<Credentials*>(n3);

        RTT::WriteBufferPort<Credentials*>* param_tx =
            new RTT::WriteBufferPort<Credentials*>(n1, 15);

        //RTT::Buffer<Credentials*, RTT::NonBlockingPolicy, RTT::BlockingPolicy>* param_tx = new RTT::Buffer<Credentials*, RTT::NonBlockingPolicy, RTT::BlockingPolicy>(1);
        //RTT::PortInterface* param_tx = (RTT::PortInterface*)new RTT::Buffer<Credentials*, RTT::NonBlockingPolicy, RTT::BlockingPolicy>(1);
//        RTT::PortInterface* param_tx = new RTT::PortInterface(n1);
 //       param_tx->createConnection(buf_param_tx);

        RTT::ReadDataPort<Credentials*>* param_rx =
            new RTT::ReadDataPort<Credentials*>(n4);

        //new
        pcol->ports()->addPort(pcol_tx, n3);

        //new
        pram->ports()->addPort(param_tx, n1);
        pram->ports()->addEventPort(param_rx, n4);

        //pram->outport = (RTT::Buffer*)param_tx;

        //new
        pram->outport = param_tx;
        pram->inport = param_rx;

        //new
        //param_tx->connectTo(pcol->request_stack);
        pcol_tx->connectTo(param_rx);
        pcol->connectPeers(pram);

        RTT::Handle h = pram->events()->setupConnection("sendGoal")
            .callback( pcol, &Protocol::addRequest,
                       pcol->engine()->events() ).handle();
        assert( h.ready() );
        h.connect();
        assert( h.connected() );

        //pcol->registerParam(pram);
       
        return true;
    }
}
