
#include "EventLoop.h"
#include "TimerQueue.h"
#include "TimeStamp.h"

#include <signal.h>
using namespace std;

using namespace net;

using namespace base;

EventLoop *g_loop;
/*void runEvery(){
    TimeStamp end(TimeStamp::now());
    printf("runEvery, Time %s\n",end.toFormatString(true).c_str());
}

void signal_handler(){
    TimeStamp end(TimeStamp::now());
    printf("signal_handler, Time %s\n",end.toFormatString(true).c_str());
}
void test(){

    EventLoop loop;
    g_loop=&loop;
    g_loop->setSignal(SIGINT,signal_handler);
    g_loop->runEvery(runEvery,1);
    printf("Priority 1 %s\n",g_loop->getEventName(1));
    printf("Priority 2 %s\n",g_loop->getEventName(2));
    printf("Priority 3 %s\n",g_loop->getEventName(3));
    g_loop->loop();


}
void test1(){
    EventLoop loop;
    g_loop=&loop;

    printf("Priority 1 %s\n",g_loop->getEventName(1));
    printf("Priority 2 %s\n",g_loop->getEventName(2));
    printf("Priority 3 %s\n",g_loop->getEventName(3));
    g_loop->setEventPriority("SignalEvent",1);
    printf("Priority 1 %s\n",g_loop->getEventName(1));
    printf("Priority 2 %s\n",g_loop->getEventName(2));
    printf("Priority 3 %s\n",g_loop->getEventName(3));
    g_loop->runEvery(runEvery,1);
    g_loop->setSignal(SIGINT,signal_handler);
    g_loop->loop();
}*/
int main(){

    // test();
        // test1();
}
