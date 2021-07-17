
#include "signal.h"
#include "EventLoop.h"
#include "TimeStamp.h"

#include <unistd.h>
#include <vector>
#include <thread>
using namespace std;

using namespace base;

using namespace net ;

EventLoop *g_loop;
int signals;
int count=0;
void print(){

    printf("recv signal\n");
    
    ++count;
    if(count == 5)g_loop->delSignal(signals);
}

void test(){

    EventLoop loop;
    g_loop=&loop;
    g_loop->setSignal(SIGINT,print);
    signals=SIGINT;
    g_loop->loop();
    


}
void print1(){
    printf("recv signals\n");
    count++;
    if(count == 5)g_loop->delSignal(signals);
}
void test3(){

    EventLoop loop;
    g_loop=&loop;
    // printf("Priority 1 %s\n",g_loop->getEventName(1));
    // printf("Priority 2 %s\n",g_loop->getEventName(2));
    // printf("Priority 3 %s\n",g_loop->getEventName(3));
    g_loop->setSignal(SIGINT,print1);
    g_loop->setSignal(SIGQUIT,print1);

    signals=SIGINT;
    g_loop->loop();

}
void thread1(){

    printf("thread1\n");
    EventLoop loop;
    // loop.setSignal(SIGQUIT,print);
    loop.loop();
}

void thread2(){
    
    printf("thread2\n");
    EventLoop loop;
    loop.setSignal(SIGINT,print);
    loop.loop();
}
void test1(){

    thread t1(thread1);
    thread t2(thread2);

    sleep(5);
    printf("send signal\n");
    t1.join();
    // t2.join();
}
int main(){

        // test();
        test3();
//


}
