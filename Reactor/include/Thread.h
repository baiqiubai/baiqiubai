
#ifndef  THREAD_BASE_H_
#define  THREAD_BASE_H_
#include "CountDownLatch.h"

#include <functional>
#include <atomic>
namespace base{

class Thread{
    public:
        using threadCallback=std::function<void()>;
            
        void stop();

        ~Thread(){
            
            if(start_){
                stop();
            }
        }
        Thread(const threadCallback &cb,std::string name="");

        void start();

        std::string name()const{return name_;}

        pthread_t threadPid()const{return threadPid_;}

        pid_t threadTid()const{return threadTid_;}
    private:
        
        void run(){
            if(threadCallback_)threadCallback_(); 
        
        }
        static void *threadfunc(void *);

        threadCallback threadCallback_;
        std::string name_;
        std::atomic<bool>  start_;
        
        CountDownLatch latch_;
        pid_t threadTid_;

        pthread_t threadPid_;

        

        static int nextThreadId_;
};






}

#endif 
