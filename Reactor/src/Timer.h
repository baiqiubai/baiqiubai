
#ifndef TIMER_NET_H_
#define TIMER_NET_H_

#include "TimeStamp.h"

#include <atomic>
#include <functional>
namespace net {


class Timer{
    public:
        using timerCallback=std::function<void()>; 
        Timer(const timerCallback &cb,
                base::TimeStamp now,
                double interval,
                bool repeat)
            :timerCallback_(cb),
            timer_(now),
            interval_(interval),
            repeat_(repeat),
            sequence_(std::atomic_fetch_add(&Num_,1))
            {
            
            
            } 

        void run(){
            
            if(timerCallback_)timerCallback_();
        }    
    
        bool repeat()const{return repeat_;}
        void resetTimer(base::TimeStamp now);
        base::TimeStamp time()const{return timer_;}
    private:
    
        timerCallback timerCallback_;
        base::TimeStamp timer_;

        const double interval_;
        const bool repeat_;


        const int sequence_;
        static std::atomic_int Num_;
        
};




}

#endif 
