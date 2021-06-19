
#include "EventLoopThreadPool.h"


namespace net {

    EventLoopThreadPool::EventLoopThreadPool(EventLoop *baseLoop)
    :baseLoop_(baseLoop),
    started_(false),
    next_(0),
    threadNums_(0)
    {
    
    
    
    
    }
    EventLoopThreadPool::~EventLoopThreadPool(){
    
            
    
    }   
    EventLoop *EventLoopThreadPool::getBaseLoop(){
        
        return baseLoop_;
    }
    void EventLoopThreadPool::start(){
        
        
        started_=true;
        for(int i=0;i<threadNums_;++i){
            
            
            // std::unique_ptr<EventThreadLoop> loop(new EventThreadLoop());
            threads_.emplace_back(std::unique_ptr<EventThreadLoop>(new EventThreadLoop()));
            loops_.push_back(threads_[i]->startLoop());  
        }
    
    
    }
    EventLoop* EventLoopThreadPool::getNextLoop(){
    
        EventLoop *loop=baseLoop_;
        if(!loops_.empty()){
        
            loop=loops_[next_];
            next_++;
            if(next_==threadNums_){
                next_=0;
            }
        
        }
        return loop;
        
    }









}
