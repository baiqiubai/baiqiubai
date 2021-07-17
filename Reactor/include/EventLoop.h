
#ifndef EVENTLOOP_NET_H_
#define EVENTLOOP_NET_H_

#include "CurrentThread.h"
#include "Mutex.h"
#include "Signal.h"

#include <vector>
#include <atomic>
#include <memory>
#include <unordered_map>
#include <functional>
namespace net {
 
namespace detail{

    int createEventFd();
    std::string getReventsName(int revents);

}


class Channel;
class Poller;
class TimerQueue;
// class Signal;
class EventLoop{

    public:
        using Functor=std::function<void()>;    
        EventLoop();
        ~EventLoop();
        void stop();    
        
        void loop();
    
        void update(Channel *channel);
        void remove(Channel *channel);


        pid_t loopTid()const{return loopTid_;}

        void runInLoop(Functor func);
        void queueInLoop(const Functor &func);
        void assertInLoop(){if(!isInLoop())abort();}
        bool isInLoop()const{return base::CurrentThread::tid()==loopTid_;}

        // void setSignal(int signals,Functor func);
        // void delSignal(int signals);
        
        void setSignal(uint32_t signo,Functor func);
        void delSignal(uint32_t signo);
        void runAt(const Functor &cb,double delay);

        void runAfter(const Functor&cb,double delay);

        void runEvery(const Functor&cb,double interval);

        
        void setPollInterval(int timeout);

    private:

        void wakeup();
        void doPendingFunctors();
    
        void readEventFd();
        std::atomic<bool> start_;
        
        std::vector<Channel*> activeChannel_;
        pid_t loopTid_;
        std::unique_ptr<Poller> poller_;

        std::unique_ptr<TimerQueue> timerQueue_;

        // std::unique_ptr<SignalInfo> signalInfo_;
        
        std::unique_ptr<Signal> signal_;
        std::unique_ptr<Channel> wakeupChannel_; //使得能从poll阻塞中唤醒

        base::MutexLock mutex_;
        
        int pollInterval_;
        std::vector<Functor> pendingFunctors_;

        std::atomic<bool> callingPendFunctors_;
};







}


#endif 

