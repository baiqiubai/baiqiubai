

#ifndef EVENTTHREADLOOP_NET_H_
#define EVENTTHREADLOOP_NET_H_

#include "Mutex.h"
#include "Condition.h"
#include "Thread.h"
namespace net {

class EventLoop;
class EventThreadLoop{
    public:

        EventThreadLoop();
        EventLoop* startLoop();


    private:
        void threadfunc();
        EventLoop *loop_;
        base::Thread thread_;
        base::MutexLock mutex_;
        base::Condition cond_;

};


}

#endif 




