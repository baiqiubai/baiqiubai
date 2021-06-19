
#include "TcpServer.h"
#include "EventLoop.h"
#include "InetAddress.h"
#include "Thread.h"
#include "Buffer.h"
#include "CurrentThread.h"
using namespace std;

using namespace net;
using namespace base;
TcpServer *g_server;
EventLoop *g_loop;
void OtherThread(){
    cout<<"OtherThread CurrentThread tid "<<CurrentThread::tid()<<"\n";
    g_server->start();
}

void OnConnection(const TcpConnectionPtr&conn){


    printf("NewConnection\n");
}
void message(const TcpConnectionPtr&conn,Buffer *buffer)
{

    printf("message Thread tid %ld\n",CurrentThread::tid());
    std::string result("hello world");
    conn->send(result);
}
int main(int argc,char**argv){


    cout<<"Loop Thread tid "<<CurrentThread::tid()<<"\n";
    InetAddress address(argv[1],atoi(argv[2]));
    EventLoop loop;
    TcpServer tcpserver(&loop,address);
    g_server=&tcpserver;
    g_server->setMessageCallback(std::bind(&message,_1,_2));
    g_server->setConnectionCallback(std::bind(&OnConnection,_1));
    Thread t(OtherThread);
    t.start();
    loop.loop();
}

