
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

    EventLoop::EventLoop()
    :start_(false),
    loopTid_(base::CurrentThread::tid()),
    poller_(newDefaultPoller()),
    timerQueue_(new TimerQueue(this)),
    signal_(new Signal(this)),
    wakeupChannel_(new Channel(this,detail::createEventFd())),
    mutex_(),
    pollInterval_(5),
    callingPendFunctors_(false)
    {
    
        wakeupChannel_->setReadCallback(std::bind(&EventLoop::readEventFd,this));
        wakeupChannel_->enableReading();
    }
    
        
     
    void EventLoop::setPollInterval(int timeout){
    
        pollInterval_=timeout;
    }    
    
    void EventLoop::setSignal(uint32_t signo,Functor cb){
        
        signal_->setSignal(signo,std::move(cb));
    }
    void EventLoop::delSignal(uint32_t signo){
    
        signal_->delSignal(signo);
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
        
        LOG_DEBUG<<"EventLoop::~EventLoop ";
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
        LOG_DEBUG<<"start Loop";
        while(start_){
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
