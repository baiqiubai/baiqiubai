
#include "EventThreadLoop.h"
#include "EventLoop.h"
using namespace base;
namespace net {

    EventThreadLoop::EventThreadLoop():
    loop_(NULL),
    thread_(std::bind(&EventThreadLoop::threadfunc,this)),
    mutex_(),
    cond_(mutex_){
    
    
    
    
    }

    EventLoop *EventThreadLoop::startLoop(){
    
        
        thread_.start();
        {
        
            MutexLockGuard lock(mutex_);
            while(loop_==NULL){
            
                cond_.wait();
            }
        }

        return loop_;
    
    }

    void EventThreadLoop::threadfunc(){
    
        EventLoop loop;
        {
        
            MutexLockGuard lock(mutex_);
            loop_=&loop;
            cond_.signal();
        }

        loop.loop();
    

    
    }





}
