
#ifndef THREADPOOL_BASE_H_
#define THREADPOOL_BASE_H_
#include "Thread.h"
#include "Mutex.h"
#include "Condition.h"

#include <vector>
#include <deque>
#include <memory>
namespace base{
class ThreadPool{

    public:

        using threadCallback=std::function<void()>;
        
        ThreadPool(const threadCallback&cb,std::string name="");
        
        void  start(int threadNum);
        
        ~ThreadPool(){
        
            if(start_){
                stop();
            }
        }

        void stop();

        void dispatchTask(threadCallback cb);

        void setTaskNum(int num){taskNum_=num;}
            
        std::string name()const{return poolName_;}

    private:

        void runThread();

        bool full();
        threadCallback threadCallback_;

        threadCallback take();
        std::string poolName_;

        std::atomic<bool> start_;
        
        int nextThreadId_;

        MutexLock mutex_;

        Condition emptyCond_;
        Condition fullCond_;

        int taskNum_;
        std::vector<std::unique_ptr<Thread>> threads_;

        std::deque<threadCallback> taskQueue_;
};
}

#endif 
