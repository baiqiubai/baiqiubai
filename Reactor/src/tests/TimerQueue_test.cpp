
#include "EventLoop.h"
#include "TimerQueue.h"
#include "TimeStamp.h"



using namespace std;

using namespace base;

using namespace net;

int count=0;
EventLoop *g_loop;
TimeStamp *g_stamp;
void runAfter(){

    TimeStamp end(TimeStamp::now());
    printf("runAfter\n");
    printf("timerCallback interval %f\n",
            timeDifference(end,*g_stamp));
}
void runEvery(){
    ++count;
    TimeStamp end(TimeStamp::now());
    printf("runEvery \n");
    printf("timerCallback interval %f\n",
            timeDifference(end,*g_stamp));
    if(count==10){
        g_loop->stop();
    }
}
void test(){

    EventLoop loop;
    g_loop=&loop;

    TimeStamp start(TimeStamp::now());
    g_stamp=&start;
    g_loop->runAfter(runAfter,3);
    g_loop->runEvery(runEvery,2);
    g_loop->loop();

    
}

void test1(){
    EventLoop loop;
    g_loop=&loop;

    TimeStamp start(TimeStamp::now());
    g_stamp=&start;

    g_loop->runEvery(runEvery,1);
    g_loop->loop();


}
int main(){

    // test();
    //
        test1();
}
