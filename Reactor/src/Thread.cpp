
#include "Thread.h"
#include "CurrentThread.h"


#include <cstring>
namespace base{

int Thread::nextThreadId_=1;

Thread::Thread(const threadCallback &cb,std::string name)
:threadCallback_(cb),
name_(name),
start_(false),
latch_(1),
threadTid_(0),
threadPid_(0){


}

void Thread::stop(){
    
    if(start_){
        
        if(pthread_join(threadPid_,NULL)){
            perror("pthread_join error");    
        
        }
        start_=false;
        
       /* if(pthread_detach(threadPid_)){
        
        }*/
    }

}

void Thread::start(){

    start_=true;
    char buf[30];
    memset(buf,'\0',sizeof(buf));
    if(pthread_create(&threadPid_,NULL,threadfunc,this)==0){
        
        snprintf(buf,sizeof(buf),"%d",nextThreadId_);

        name_+=buf;
        nextThreadId_++;

        latch_.wait();
        
    }else {
    
        perror("pthread_create error");
    }



}
void *Thread::threadfunc(void *arg){

    Thread *thread=static_cast<Thread*>(arg);

    thread->threadTid_=CurrentThread::tid();
    thread->latch_.countDown();
    thread->run();
    pthread_exit(NULL);
    
}


}
