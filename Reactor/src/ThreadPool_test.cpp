

#include "ThreadPool.h"
#include "CurrentThread.h"

#include <unistd.h>
using namespace std;
using namespace base;
using namespace CurrentThread;

ThreadPool *pool;
using threadCallback=std::function<void()>;
void print() {
    
    cout<<"print tid "<<CurrentThread::tid()<<" pid "<<getpid()<<"\n";
}

void threadfunc(){

    cout<<"threadfunc tid "<<CurrentThread::tid()<<" pid "<<getpid()<<"\n";
    
    CurrentThread::sleepUsec(1000);
}

void setThreadNum(int num){
    
    
    pool->start(num);

}

void dispatchTaskNum(int num,threadCallback cb){


    for(int i=0;i<num;++i){
    
        pool->dispatchTask(cb);
        // sleep(1);
    }
}

int main()
{
   /* {

        cout<<"Main tid "<<CurrentThread::tid()<<" pid "<<getpid()<<"\n";
        ThreadPool Pool(print,"First");
        pool=&Pool;
        setThreadNum(0);  //无线程
        dispatchTaskNum(5,threadfunc);
    
    }
    sleep(1); 
    cout<<"=====Second======="<<"\n";
    {

        ThreadPool Pool(print,"Second");
        pool=&Pool;
        setThreadNum(1); 
        dispatchTaskNum(3,threadfunc);
        usleep(100);
        // Pool.stop();
    }

    cout<<"=======Third========"<<"\n";
    {
        ThreadPool Pool(print,"Third");
        pool=&Pool;
        setThreadNum(5); 
        Pool.setTaskNum(5);
        dispatchTaskNum(50,threadfunc);
        
        sleep(1);
        // sleep(3);
    
    }*/ 
    cout<<"=====Fourth====="<<"\n";
    {
        ThreadPool p(print,"Fourth");
        pool=&p;
        setThreadNum(20);
        p.setTaskNum(5);
        dispatchTaskNum(5,threadfunc);
    
        sleep(1);
    }
}
