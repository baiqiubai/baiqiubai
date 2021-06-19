
#include "TimerQueue.h"
// #include "EventLoop.h"
#include "Channel.h"
#include "Timer.h"
#include "Logger.h"
#include "LogStream.h"

#include <sys/time.h>
#include <unistd.h>
#include <assert.h>
namespace net {

namespace detail {

    int createTimerfd(){
        
        int timerfd=::timerfd_create(CLOCK_REALTIME,TFD_NONBLOCK|TFD_CLOEXEC);

        if(timerfd < 0){
            perror("timerfd_create error"); 
            // LOG_ERROR<<"timerfd_create error";
        }
        
        return timerfd;
    } 
    struct timespec howMuchTimer(const base::TimeStamp &when){
        uint64_t microseconds=when.microSeconds()-base::TimeStamp::now().microSeconds();

        struct timespec spec;
        if(microseconds < 100){
        
            microseconds=100;
        }
            
            
        spec.tv_sec=static_cast<time_t>(microseconds/base::TimeStamp::kSecondsToMicroSeconds);
        spec.tv_nsec=static_cast<long>(microseconds %base::TimeStamp::kSecondsToMicroSeconds*1000);
        return spec;
    
    }
        
    struct timespec resetTimer(const base::TimeStamp &now){
    
        struct timespec spec;

        int64_t microseconds=now.microSeconds();
    
        int seconds=microseconds /base::TimeStamp::kSecondsToMicroSeconds;

        int64_t nanoseconds=microseconds % base::TimeStamp::kSecondsToMicroSeconds * 1000;

        spec.tv_sec=static_cast<time_t>(seconds);
        spec.tv_nsec=static_cast<long>(nanoseconds);
    
        return spec;
    }
    void setTimer(int timerfd,const base::TimeStamp &now){
    
        struct itimerspec newspec;
        memset(&newspec,'\0',sizeof(newspec));

        newspec.it_value=resetTimer(now);
        // newspec.it_value=howMuchTimer(now);
        if(::timerfd_settime(timerfd,TFD_TIMER_ABSTIME,&newspec,NULL)==-1){
            perror("timerfd_settime error");        
        }
    
    }
    
    void readTimerfd(int timerfd){
again:
        uint64_t one;
        size_t n=::read(timerfd,&one,sizeof(one));
    
        if(n!=sizeof(one)){
            if(errno==EINTR)
                goto again;
            else if(errno!=EAGAIN&&errno!= EWOULDBLOCK)
                perror("read error");
            // LOG_ERROR<<"readTimerfd error";
        }
    }
}
    TimerQueue::TimerQueue(EventLoop *loop)
    :loop_(loop),
    timerFd_(detail::createTimerfd()),
    timerChannel_(new Channel(loop_,timerFd_)){
   

        timerChannel_->setReadCallback(std::bind(&TimerQueue::handlerRead,this));
        timerChannel_->enableReading();
    
    }
    
    bool TimerQueue::insert(Timer *timer){
    
        bool earliest=false;

        
        if(timers_.empty()||(timer->time()<(*timers_.begin()).second->time()))
            earliest=true;

     
        timers_.insert(Entry(timer->time(),timer));
        return earliest;

    
    }
    void TimerQueue::addTimer(Timer *timer){
        bool earliest=insert(timer);
        if(earliest)
            detail::setTimer(timerFd_,timer->time());


    }
    std::vector<TimerQueue::Entry> TimerQueue::getExpired(const base::TimeStamp &now){
    
        std::vector<Entry> res;
        

        Entry entry(now,reinterpret_cast<Timer*>(INTPTR_MAX));
        
        auto pos=timers_.lower_bound(entry);

        assert(pos==timers_.end()||now< (*pos).first);


        // assert(now < (*pos).first);
        // asser((*pos).first >now);
    
        std::copy(timers_.begin(),pos,std::back_inserter(res));
        
        timers_.erase(timers_.begin(),pos);

        return res;        
    
    } 
    void TimerQueue::reset(std::vector<TimerQueue::Entry> &expired
            ,const base::TimeStamp &now){


        for(auto it=expired.begin();it!=expired.end();++ it){
            
            if(it->second->repeat()){
                it->second->resetTimer(now);
                insert(it->second);
            }else {
                
                delete it->second;
            }
        
        }
        base::TimeStamp nextExpired;    
        if(!timers_.empty()){
        
            nextExpired=timers_.begin()->second->time();
        }
        if(nextExpired.microSeconds()!=0){
            detail::setTimer(timerFd_,nextExpired);
        }

    
    
    
    }
    void TimerQueue::handlerRead(){
        std::vector<Entry> expired; 
        detail::readTimerfd(timerFd_);    

        base::TimeStamp now(base::TimeStamp::now());
        expired=getExpired(now);
        for(auto timer:expired){
            timer.second->run();
        }
        
        reset(expired,now);
    }






}
