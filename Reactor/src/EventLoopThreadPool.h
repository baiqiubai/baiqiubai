

#ifndef EVENTLOOPTHREADPOOL_NET_H_
#define EVENTLOOPTHREADPOOL_NET_H_


#include "EventLoop.h"
#include "EventThreadLoop.h"

#include <vector>
#include <memory>
namespace net {

class EventLoop;
class EventThreadLoop;
class EventLoopThreadPool{


    public:
    
        EventLoopThreadPool(EventLoop *baseLoop);
        ~EventLoopThreadPool();
    

        void setThreadNum(int num){threadNums_=num;}
        void start();
        EventLoop *getNextLoop();
        EventLoop *getBaseLoop();

    private:
        EventLoop *baseLoop_;
        bool started_;
        int next_;
        int threadNums_;
        std::vector<std::unique_ptr<EventThreadLoop>> threads_;
        std::vector<EventLoop*> loops_;
    
};




}


#endif 
