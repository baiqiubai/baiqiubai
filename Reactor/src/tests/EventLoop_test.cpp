
#include <iostream>
#include "EventLoop.h"
#include "Thread.h"
#include "TimeStamp.h"

#include <unistd.h>
using namespace std;
using namespace base;

using namespace net;
EventLoop *g_loop;

void run1(){

    cout<<"run1 CurrentThread tid "<<CurrentThread::tid()<<"\n"; 
}
void OtherThread(){


    cout<<"OtherThread CurrentThread tid "<<CurrentThread::tid()<<"\n";
    g_loop->runInLoop(run1);
}

void run2(){

    cout<<"run2 CurrentThread tid "<<CurrentThread::tid()<<"\n"; 
    g_loop->queueInLoop(run1);
    

}
void OtherThread1(){//not Loop Thread 


    cout<<"OtherThread CurrentThread tid "<<CurrentThread::tid()<<"\n";
    g_loop->runInLoop(run2);

}


void test1(){


    g_loop->runInLoop(run1);
}
void test2(){
    
    g_loop->setPollInterval(-1);
    Thread t(OtherThread);
    t.start();

}
void test3(){
    g_loop->setPollInterval(-1);
    Thread t(OtherThread1);
    t.start();

}

void test4(){
    
    
  cout<<"test4 CurrentThread tid  "<<CurrentThread::tid()<<"\n";
  g_loop->stop();
}

void NotIoThread(){

    CurrentThread::sleepUsec(100);
    cout<<"NotIoThread CurrentThread tid  "<<CurrentThread::tid()<<"\n";
    g_loop->stop();
    // g_loop->runAfter(test4,3);
}

void test5(){

    Thread t(NotIoThread);
    t.start();
    g_loop->loop();

}
int main(){

    cout<<"Main thread CurrentThread tid  "<<CurrentThread::tid()<<"\n";
    EventLoop loop;
    g_loop=&loop;
    g_loop->setPollInterval(-1); //永久阻塞
    TimeStamp start(TimeStamp::now());
    test5();
    // g_loop->runAfter(test4,3);
    // test1();
        // test2();
    // test3();
    TimeStamp end(TimeStamp::now());
    printf("interval %f\n",timeDifference(start,end));
}
