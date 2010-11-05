//#include "hardware.hpp"
namespace ACES {

    template <class T>
    Hardware<T>::Hardware(std::string cfg, std::string args) :
        taskCfg(cfg),
        RTT::TaskContext(name),
        txUpStream("txUpStream"),
        dsQueue(10),
        usQueue(10)
    {
        this->events()->addEvent(&txUpStream, "txUpStream", "word",
                                 "Recieved Data");

        this->setActivity( new RTT::Activity( priority, 1.0/freq, 0,
                                              name));
    }

    template <class T>
    bool Hardware<T>::rxBus(){
        return true;
    }

    template <typename T>
    HWord<T>* Hardware<T>::processUSQueue(){
        HWord<T>* w = NULL; 
        usQueue.dequeue(w);
        return w;
    }

    template <class T>
    Message<T>* Hardware<T>::processDSQueue(){
        Message<T>* m = NULL;
        dsQueue.dequeue(m);
        //getDSQelement();
        return m;
    }

    template <class T>
    void Hardware<T>::updateHook(){
        //RTT::Logger::log() << "hw sent" << RTT::endlog();
        Message<T>* m = NULL;
        while(m = processDSQueue()){
            RTT::Logger::log() << RTT::Logger::Debug << "(HW) got DS"
                               << RTT::endlog();
            txBus(m);
        }
        rxBus();
        HWord<T>* p = NULL;
        while(p = processUSQueue()){
            RTT::Logger::log() << RTT::Logger::Debug << "(HW) got US"
                               << RTT::endlog();
            txUpStream(p);
        }
    }

    template <class T>
    bool Hardware<T>::txBus(Message<T>* m){
        while( m->size() ){
            HWord<T>* w = m->Pop();
            usQueue.enqueue(w);
            //TODO - Delete the word
        }
        return true;
    }

    template <class T>
    bool Hardware<T>::rxDownStream(Message<T>* m){
        dsQueue.enqueue(m);
        return true;
    }
}
