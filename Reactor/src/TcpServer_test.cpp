
#include "TcpServer.h"
#include "EventLoop.h"
#include "InetAddress.h"
#include "Thread.h"
#include "Buffer.h"
#include "CurrentThread.h"
#include "Logger.h"
#include "LogStream.h"

using namespace std;

using namespace net;
TcpServer *g_server;
EventLoop *g_loop;

void OnConnection(const TcpConnectionPtr&conn){


    printf("OnConnection Thread tid %d\n",base::CurrentThread::tid());
}
void message(const TcpConnectionPtr&conn,Buffer *buffer)
{

    printf("message Thread tid %d\n",base::CurrentThread::tid());
    
    char buf[10]={0};
    memset(buf,'a',sizeof(buf));
    conn->send(buf,sizeof(buf));
}
void debug(){
    base::Logger::setLogLevel(base::Logger::DEBUG);
}
int main(int argc,char**argv){

    
    // int num=0;
    // if(argc==3)num=atoi(argv[3]);
    cout<<"IO Thread tid "<<base::CurrentThread::tid()<<"\n";
    InetAddress address(argv[1],atoi(argv[2]));
    EventLoop loop;
    TcpServer tcpserver(&loop,address);
    g_server=&tcpserver;
    
    // setThreadNum_(num);
    tcpserver.setThreadNum(atoi(argv[3]));//设置三个线程
    g_server->setMessageCallback(std::bind(&message,_1,_2));
    g_server->setConnectionCallback(std::bind(&OnConnection,_1));
    debug();
    g_server->start();
    loop.loop();
}
