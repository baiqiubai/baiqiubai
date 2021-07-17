
#include "Thread.h"
#include "CurrentThread.h"
#include "TimeStamp.h"
#include <unistd.h>
using namespace std;
using namespace base;

using namespace CurrentThread;

void threadfunc(){

    std::cout<<"CurrentThread tid "<<CurrentThread::tid()<<" pid "<<getpid()<<"\n";
    
    sleep(5);
}
int main(){

    

    std::cout<<"Main thread tid "<<CurrentThread::tid()<<" pid "<<getpid()<<"\n";
    Thread t(threadfunc,"First Thread");
    
   TimeStamp now(TimeStamp::now());
    t.start();
    std::cout<<"name" <<t.name()<<" " <<"  tid  "<<t.threadTid()<<" pid "<<t.threadPid()<<"\n";
    t.stop();
   TimeStamp end(TimeStamp::now());

    printf("interval %f\n",timeDifference(end,now));


    {
    
        Thread t(threadfunc,"Second Thread");

        t.start();
        std::cout<<"name" <<t.name()<<" " <<"  tid  "<<t.threadTid()<<" pid "<<t.threadPid()<<"\n";

    
    }
    // sleep(3);
}

