
#include "Signal.h"
#include "EventLoop.h"
#include <signal.h>
#include <arpa/inet.h>
using namespace net;
using namespace base;

EventLoop *g_loop;
int cnt=0;
void sigInt(){

    printf("recv SIGINT ");
    ++cnt;
    if(cnt==10)g_loop->delSignal(SIGINT);
}
void sigQuit(){
    printf("recv SIGQUIT ");
}
void Addsignal(){

   g_loop->setSignal(SIGINT,std::bind(&sigInt)); 
   g_loop->setSignal(SIGQUIT,std::bind(&sigQuit));
}
int main(){
    
    EventLoop loop;
    g_loop=&loop;
    Addsignal();
    g_loop->loop();
}
