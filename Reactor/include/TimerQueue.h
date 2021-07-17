
#ifndef TIMERQUQUE_NET_H_
#define TIMERQUQUE_NET_H_

#include "TimeStamp.h"

#include <sys/timerfd.h>

#include <memory>
#include <set>
#include <vector>
namespace net {

namespace detail {

    int createTimerfd();
        
        
    struct timespec howMuchTimer(const base::TimeStamp &when);
    struct timespec resetTimer(const base::TimeStamp &now);
    void setTimer(int timerfd,const base::TimeStamp &now);
    
    void readTimerfd(int timerfd);
        


}   

class EventLoop;
class Channel;
class Timer;
class TimerQueue{

    public:
        TimerQueue(EventLoop *loop);
            
        
        int timerfd()const{return timerFd_;}

        void addTimer(Timer *timer);
        void cancel(Timer *timer);
    private:

        void cancelTimer(Timer *timer);
        void handlerRead();
        using Entry=std::pair<base::TimeStamp,Timer*>; 

        
        using TimerSet=std::set<Entry>;

        std::vector<Entry> getExpired(const base::TimeStamp &now);

        bool insert(Timer *timer);
        void reset(std::vector<Entry> &expired,
                const base::TimeStamp &now);
        EventLoop *loop_;

        const int timerFd_;
        std::unique_ptr<Channel> timerChannel_;
        
        TimerSet timers_;
        std::set<Timer*> cancelTimers_;
        
        bool callingExpiredTimers_;
        
};

}

#endif 
