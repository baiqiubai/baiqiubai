
#include "EventThreadLoop.h"
#include "EventLoop.h"
#include "CurrentThread.h"
#include "InetAddress.h"
#include "TcpServer.h"
#include "Buffer.h"

#include <unistd.h>
using namespace net;
using namespace std;

using namespace base;
void onConnection(const TcpConnectionPtr&conn){


    cout<<"Newserverection CurrentThread tid "<<CurrentThread::tid()<<"\n";


}

void message(const TcpConnectionPtr &conn,Buffer *buffer){


    cout<<"message "<<CurrentThread::tid()<<"\n";
    std::string result("hello wolrd");

    conn->send(result);

}
int main(int argc,char**argv){


    
    cout<<"Main thread "<<CurrentThread::tid()<<"\n";

    InetAddress address(argv[1],atoi(argv[2]));
    EventThreadLoop ThreadLoop;
    EventLoop *loop=ThreadLoop.startLoop();
    TcpServer server(loop,address);
    server.setMessageCallback(std::bind(&message,_1,_2));
    server.setConnectionCallback(std::bind(&onConnection,_1));
    server.start();
    sleep(50);
}
