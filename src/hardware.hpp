#ifndef ACES_HARDWARE_HPP
#define ACES_HARDWARE_HPP

#include <fstream>
#include <iostream>

#include <rtt/TaskContext.hpp>
#include <rtt/Event.hpp>
#include <rtt/Ports.hpp>
#include <rtt/Logger.hpp>
#include <rtt/Activity.hpp>

#include "message.hpp"
#include "protocol.hpp"
#include "taskcfg.hpp"
#include "word.hpp"

namespace ACES {
    template <class T>
    class Protocol;

    template <class T>
    class Hardware : protected taskCfg,
                     public RTT::TaskContext {
        public:
            Hardware(std::string cfg, std::string args);
            //Hardware(std::string name);
            void updateHook();

            bool rxDownStream(Message<T>* m);
            RTT::Event<void( Word<T> )> txUpStream;

            virtual bool txBus(Message<T>* m);
            virtual bool rxBus();

            virtual Word<T> processUSQueue();
            //T, read, write
            RTT::Queue<T, RTT::BlockingPolicy,
                       RTT::BlockingPolicy> usQueue(10);
            //Word<T> getUSQelement();
            //std::deque< Word<T> > usQueue;
            //RTT::OS::Mutex usqGuard; 

            virtual Message<T>* processDSQueue();
            RTT::Queue<T, RTT::BlockingPolicy,
                       RTT::BlockingPolicy> dsQueue(10);
            //Message<T>* getDSQelement();
            //std::deque< Message<T>* > dsQueue;
            //RTT::OS::Mutex dsqGuard; 

            bool subscribeProtocol(Protocol<T>* p);
            virtual bool subscribeController(Controller* c) = 0;
    };

    template <class T>
    Hardware<T>::Hardware(std::string cfg, std::string args) :
        taskCfg(cfg),
        RTT::TaskContext(name),
        txUpStream("txUpStream"),
        dsQueue(),
        usQueue()
    {
        this->events()->addEvent(&txUpStream, "txUpStream", "word",
                                 "Recieved Data");

        this->setActivity( new RTT::Activity( priority, 1.0/freq, 0,
                                              name));
    }

    template <typename T>
    Word<T> Hardware<T>::getUSQelement(){
        RTT::OS::MutexLock lock(usqGuard);
        Word<T> w = usQueue.front();
        usQueue.pop_front();
        return w;
    }

    template <typename T>
    Word<T> Hardware<T>::processUSQueue(){
        return getUSQelement();
    }

    template <class T>
    Message<T>* Hardware<T>::getDSQelement(){
        RTT::OS::MutexLock lock(dsqGuard);
        Message<T>* m = dsQueue.front();
        dsQueue.pop_front();
        return m; 
    }

    template <class T>
    Message<T>* Hardware<T>::processDSQueue(){
        Message<T>* m = getDSQelement();
        return m;
    }

    template <class T>
    void Hardware<T>::updateHook(){
        //RTT::Logger::log() << "hw sent" << RTT::endlog();
        Message<T>* m = NULL;
        while(dsQueue.size()){
            m = processDSQueue();
            if(m){
                RTT::Logger::log() << RTT::Logger::Debug << "(HW) got DS"
                                   << RTT::endlog();
            txBus(m);
            }
        }
        rxBus();
        Word<T> p;
        while(usQueue.size()){
            p = processUSQueue();
            if(p){
                RTT::Logger::log() << RTT::Logger::Debug << "(HW) got US"
                                   << RTT::endlog();
            txUpStream(p);
            }
        }
    }

    template <class T>
    bool Hardware<T>::txBus(Message<T>* m){
        //Goal* g = NULL;
        while( m->size() ){
            //for(std::list<Goal*>::iterator it = m->goalList.begin();
            //    it != m->goalList.end();
            //    it++)
            //{
                ACES::Word<T> w = m.Pop();
                //ACES::ProtoWord* w =
                //    (ACES::ProtoWord*)(new ACES::Word<Goal*>(*it));
                { RTT::OS::MutexLock lock(usqGuard);
                  usQueue.push_back(w);
                }
        }
        return true;
    }

    template <class T>
    bool Hardware<T>::rxDownStream(Message<T>* m){
        RTT::OS::MutexLock lock(dsqGuard);
        dsQueue.push_back(m);
        //m->printme();
        return true;
    }

}    
#endif
