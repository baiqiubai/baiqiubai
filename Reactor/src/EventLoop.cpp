
#include "EventLoop.h"
#include "Poller.h"
#include "Channel.h"
#include "TimerQueue.h"
#include "Timer.h"
#include "signal.h"
#include "Logger.h"
#include "LogStream.h"
#include "DefaultPoller.h"

#include <unistd.h>
#include <sys/eventfd.h>
#include <assert.h>
#include <cstring>

using namespace base;
namespace net {

namespace detail{

    int createEventFd(){
    
        int fd=::eventfd(0,EFD_NONBLOCK|EFD_CLOEXEC);
        if(fd < 0)
            LOG_ERROR<<"createEventFd error";
        return fd;
    }


}

const char *EventName[3]=
{
    "SignalEvent",
    "TimerEvent",
    "OtherEvent",
};
    EventLoop::EventLoop()
    :start_(false),
    loopTid_(base::CurrentThread::tid()),
    poller_(newDefaultPoller()),
    timerQueue_(new TimerQueue(this)),
    signalInfo_(new SignalInfo(this)),
    wakeupChannel_(new Channel(this,detail::createEventFd())),
    mutex_(),
    pollInterval_(5),
    callingPendFunctors_(false)
    {
    
        wakeupChannel_->setReadCallback(std::bind(&EventLoop::readEventFd,this));
        wakeupChannel_->enableReading();
        // poller_->setSignalSocketPair(signalInfo_->readSocket());
        // poller_->setTimerFd(timerQueue_->timerfd());
        // initDefaultPriority();
        // priorityQueue_.reserve(3);
    }
    
   /* std::vector<Channel*> EventLoop::getVecByEventName(const char*event){
        

        if(strcmp(event,"SignalEvent")==0){
                return signalChannel_;
        }else if(strcmp(event,"TimerEvent")==0){
            return timerChannel_;
        }else {
            return activeChannel_;
        }   
         
    }
    void EventLoop::pushPriorityQueue(){
        
        priorityQueue_.emplace_back(getVecByEventName(defaultPriority_[1]));
        priorityQueue_.emplace_back(getVecByEventName(defaultPriority_[2]));
        priorityQueue_.emplace_back(getVecByEventName(defaultPriority_[3]));

    }
    void EventLoop::foreachPriorityQueue(){
    
        size_t size=priorityQueue_.size();
        for(int i=0;i<size;++i){
            if(!priorityQueue_[i].empty())foreachEventChannel(priorityQueue_[i]);
        }
        priorityQueue_.clear();
    }
    void EventLoop::foreachEventChannel(const std::vector<Channel*> &channel){
        for(auto ch:channel){ 
            ch->handlerEvent();
        }
    
    }
    void EventLoop::initDefaultPriority(){
    
        
        defaultPriority_[1]=EventName[1]; //信号
        defaultPriority_[2]=EventName[2];//定时器
        defaultPriority_[3]=EventName[0];//其他任务 比如新连接
    }
    int EventLoop::getEventPriority(const char*event){
    
        for(auto &it:defaultPriority_){
            
            if(strcmp(it.second,event)==0){
                
                return it.first;
            }
        }
        return -1;
            
    }
    const char *EventLoop::getEventName(int priority){
    
        assert(priority > 0 && priority <=3);
    
        return defaultPriority_[priority];
    
    }
    void EventLoop::setEventPriority(const char*event,int priority){
        
        assert(priority>0 && priority<= 3);
        
        const char *swapEvent=defaultPriority_[priority]; //要交换的event类型

        int swapPriority=getEventPriority(event);//event的原本优先级

        defaultPriority_[priority]=event;

        defaultPriority_[swapPriority]=swapEvent;
    }        
        
    */
     
    void EventLoop::setPollInterval(int timeout){
    
        pollInterval_=timeout;
    }    
    
    void EventLoop::setSignal(int signals,Functor cb){
    
        signalInfo_->setSignal(signals,std::move(cb));
        
    }
    void EventLoop::delSignal(int signals){
    
        signalInfo_->delSignal(signals); 
    }
    void EventLoop::update(Channel *channel){
    
        poller_->update(channel);
    }
    void EventLoop::remove(Channel *channel){
    
        poller_->remove(channel);
    }
    void EventLoop::stop(){
        
        start_=false;
        if(!isInLoop()){ //非IO线程调用 poll此时阻塞 需要唤醒
            wakeup();
        }
    }

    EventLoop::~EventLoop(){
        
        if(start_){
            stop();
        }
    }

    void EventLoop::queueInLoop(const Functor &func){
            
        {
            
            MutexLockGuard lock(mutex_);
            pendingFunctors_.push_back(func);
        }

        //callingPendFunctors 为了防止在dopend再次调用queueInLoop 导致不能唤醒
        if(!isInLoop()||callingPendFunctors_){
            wakeup();
        }
    }

    void EventLoop::readEventFd(){
         
        uint64_t one;
        ssize_t n=::read(wakeupChannel_->fd(),&one,sizeof(one));
        if(n!=sizeof(one)){
            
            LOG_ERROR<<"readEventFd error";
        }
    
    }
    void EventLoop::wakeup(){
    
        uint64_t one=1;
        ssize_t n=::write(wakeupChannel_->fd(),&one,sizeof(one));
        if(n !=sizeof(one)){
            
            LOG_ERROR<<"wakeup error";
        }
        
    }
    void EventLoop::runInLoop(Functor func){
    
        if(isInLoop()){
            func();
        }else {
            queueInLoop(func);
        }


    }

    void EventLoop::doPendingFunctors(){
        
        std::vector<Functor> functors;
        
        {
                
            base::MutexLockGuard lock(mutex_);
            pendingFunctors_.swap(functors);
            
        }
        callingPendFunctors_=true;
        for(auto &functor:functors){
            functor();
        }
        callingPendFunctors_=false;
    
    }
    void EventLoop::loop(){
     
        assertInLoop();
        start_=true;
        while(start_){
            // poller_->poll(&signalChannel_,&timerChannel_,&activeChannel_,pollInterval_);
            // pushPriorityQueue();
            // foreachPriorityQueue();
              
            // signalChannel_.clear();
            // timerChannel_.clear();
            //
            poller_->poll(&activeChannel_,pollInterval_);

            for(Channel *channel:activeChannel_)
                channel->handlerEvent();
            activeChannel_.clear();
            doPendingFunctors();
        } 
    

    }
    
    void EventLoop::runEvery(const Functor &cb,double interval){
        
        base::TimeStamp every(addTime(base::TimeStamp::now(),interval));
        Timer * timer=new Timer(cb,every,interval,true);
        timerQueue_->addTimer(timer);
         
    }
    void EventLoop::runAfter(const Functor&cb,double delay){
            
        base::TimeStamp after(addTime(base::TimeStamp::now(),delay));
        Timer *timer =new Timer(cb,after,delay,false);
        timerQueue_->addTimer(timer);
        
         
    }


}
