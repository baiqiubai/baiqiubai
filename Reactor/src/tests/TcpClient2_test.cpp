
#include "Thread.h"
#include "EventLoop.h"
#include "TcpClient.h"

using namespace net;
using namespace base;
EventLoop *g_loop;
void threadfunc(){

    
     




}
int main( int argc,char **argv){
    EventLoop loop;
    g_loop=&loop;
    InetAddress address(argv[1],atoi(argv[2]));
    Thread t(threadfunc);
    t.start(); 
}
