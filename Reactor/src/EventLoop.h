
#ifndef EVENTLOOP_NET_H_
#define EVENTLOOP_NET_H_

#include "CurrentThread.h"
#include "Mutex.h"


#include <vector>
#include <atomic>
#include <memory>
#include <unordered_map>
#include <functional>
namespace net {
 
namespace detail{

    int createEventFd();

}


class Channel;
class Poller;
class TimerQueue;
class SignalInfo;
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

        void setSignal(int signals,Functor func); 
        void delSignal(int signals);

        void runAt(const Functor &cb,double delay);

        void runAfter(const Functor&cb,double delay);

        void runEvery(const Functor&cb,double interval);

        //优先级 为 1 2 3 
        void setEventPriority(const char *event,int priority);
        int getEventPriority(const char*filename);
        
        const char * getEventName(int priority);

        void pushPriorityQueue();
        
        std::vector<Channel*> getVecByEventName(const char*event);
        void setPollInterval(int timeout);

    private:

        void wakeup();
        void readEventFd();
        void foreachEventChannel(const std::vector<Channel*> &channel);
        void foreachPriorityQueue();
        
        void initDefaultPriority();
        void doPendingFunctors();
        using ChannelVec=std::vector<Channel*>;
        
        ChannelVec activeChannel_;
        ChannelVec signalChannel_;
        ChannelVec timerChannel_;
        std::atomic<bool> start_;
        
        pid_t loopTid_;
        std::unique_ptr<Poller> poller_;

        std::unique_ptr<TimerQueue> timerQueue_;

        std::unique_ptr<SignalInfo> signalInfo_;
        
        std::unique_ptr<Channel> wakeupChannel_; //使得能从poll阻塞中唤醒

        base::MutexLock mutex_;
        
        int pollInterval_;
        std::unordered_map<int,const char*> defaultPriority_;
        std::vector<Functor> pendingFunctors_;

        std::atomic<bool> callingPendFunctors_;
        std::vector<std::vector<Channel*>> priorityQueue_;
};







}


#endif 

