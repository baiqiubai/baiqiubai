
#include "TcpServer.h"
#include "EventLoop.h"
#include "Buffer.h"

#include <unistd.h>
#include <sys/resource.h>
using namespace net ;
using namespace std;

void NewConnection(const TcpConnectionPtr&conn){


    cout<<"TcpConnectionPtr tid "<<conn->getLoop()->loopTid()<<"\n";
    printf("localAddress ip %s port %ld\n",conn->localAddress().ip().data(),
            conn->localAddress().port());
    
    printf("peerAddress ip %s port %ld\n",conn->peerAddress().ip().data(),
            conn->peerAddress().port());
}
void WriteComplete(const TcpConnectionPtr&conn){

    printf("WriteCompleteCallback\n");

}
void message(const TcpConnectionPtr &conn,Buffer *buffer){
    
   
    
    char buf[500000];
    memset(buf,'a',sizeof(buf));
    std::string message(buf);
    for(int i=0;i<10;++i){
        sleep(1);
        conn->send(message);
    }
    conn->send("TCP REACTOR");
}
int main(int argc,char **argv){

    InetAddress address(argv[1],atoi(argv[2]));

    EventLoop loop;
    loop.setPollInterval(-1);
    TcpServer server(&loop,address);
    server.setThreadNum(2);
    cout<<"server Loop tid "<<server.getLoop()->loopTid()<<"\n";
    server.setWriteCompleteCallback(std::bind(&WriteComplete,_1));
    server.setMessageCallback(std::bind(&message,_1,_2));
    server.setConnectionCallback(std::bind(&NewConnection,_1));
    server.start();

    loop.loop();



}
