
#include "CurrentThread.h"
#include "TimeStamp.h"
#include "Mutex.h"

#include <vector>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
using namespace base;
using namespace CurrentThread;

MutexLock mutex_;
void *threadfunc(void *){

    MutexLockGuard lock(mutex_);
    // CurrentThread::sleepUsec(1000);
    TimeStamp start(TimeStamp::now());
    std::cout<<"start "<<start.microSeconds()<<"\n";
    std::cout<<"tid "<<CurrentThread::tid()<<" pid "<<getpid()<<"\n";
    CurrentThread::sleepUsec(100000);
    TimeStamp end(TimeStamp::now());
    std::cout<<"end "<<end.microSeconds()<<"\n";

    printf("interval %f\n",timeDifference(start,end));

    pthread_exit(NULL);
}
int main(int argc,char**argv){


    int num=atoi(argv[1]);
    pthread_t pid[num];

    for(int i=0;i<num;++i){
        
        if(pthread_create(&pid[i],NULL,threadfunc,NULL)){
            perror("pthread_create error");
        }
        if(pthread_detach(pid[i])){
            perror("pthread_detch error");
        }
    }


    sleep(5);

}
