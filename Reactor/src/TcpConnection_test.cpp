
#include "TcpServer.h"
#include "EventLoop.h"
#include "Buffer.h"
#include "Logger.h"
#include "LogStream.h"

#include <signal.h>
#include <unistd.h>
#include <sys/resource.h>

#include <cstring>
using namespace net ;
using namespace std;

size_t sendTotalSize=0; 
size_t recvTotalSize=0;
void NewConnection(const TcpConnectionPtr&conn){


    cout<<"TcpConnectionPtr tid "<<conn->getLoop()->loopTid()<<"\n";
    printf("localAddress ip %s port %d\n",conn->localAddress().ip().data(),
            conn->localAddress().port());
    
    printf("peerAddress ip %s port %d\n",conn->peerAddress().ip().data(),
            conn->peerAddress().port());
    
/*    char buf[500000];
    memset(buf,'a',sizeof(buf));
    for(int i=0;i<20;++i){
        conn->send(buf,sizeof(buf));
        sendTotalSize+=sizeof(buf);
    }*/
}   
void WriteComplete(const TcpConnectionPtr&conn){

    printf("WriteCompleteCallback SendTotalSize %ld\n",sendTotalSize);

}
void message(const TcpConnectionPtr &conn,Buffer *buffer){
    
        
      printf("recv message bytes %ld",buffer->totalSize());

      size_t size=buffer->totalSize();
      recvTotalSize+=size;  
      char buf[size+1]={0};
      buffer->bufferRead(buf,sizeof(buf));
      printf("remain Bytes %ld recvTotalSize %ld\n",buffer->totalSize(),recvTotalSize);
}
void debug(){
    
    base::Logger::setLogLevel(base::Logger::LogLevel::DEBUG);
}
int main(int argc,char **argv){
    
    
    debug();
    InetAddress address(argv[1],atoi(argv[2]));
    signal(SIGPIPE,SIG_IGN);
    EventLoop loop;
    loop.setPollInterval(-1);
    TcpServer server(&loop,address);
    
    server.setThreadNum(0);
    // server.setThreadNum(2);
    // cout<<"server Loop tid "<<server.getLoop()->loopTid()<<"\n";
    server.setWriteCompleteCallback(std::bind(&WriteComplete,_1));
    server.setMessageCallback(std::bind(&message,_1,_2));
    server.setConnectionCallback(std::bind(&NewConnection,_1));
    server.start();

    loop.loop();



}
