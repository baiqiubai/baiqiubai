
#include "TcpClient.h"
#include "InetAddress.h"
#include "EventLoop.h"
#include "TcpConnection.h"
#include "Logger.h"
#include "LogStream.h"

#include <unistd.h>
using namespace net;
using namespace std;
size_t sendtotalSize=0;
size_t recvTotalSize=0;
void NewConnection(const TcpConnectionPtr &conn){

    InetAddress &address=conn->peerAddress();
    cout<<"NewConnection PeerAddress "<<address.ip()<<" Port "
        <<address.port()<<"\n";
    
/*    char buf[200000];
    memset(buf,'c',sizeof(buf));
    for(int i=0;i<7;++i){
        conn->send(buf,sizeof(buf));
        sendtotalSize+=sizeof(buf);
    }*/
}

void WriteComplete(const TcpConnectionPtr &conn){

    printf("WriteComplete sendtotalSize %ld\n",sendtotalSize);
}
void NewMessage(const TcpConnectionPtr &conn,Buffer *buffer){
    
    size_t size=buffer->totalSize();
    
    char buf[size+1]={0};
    recvTotalSize+=size;
    printf("Recv Message totalSize %ld ", buffer->totalSize());
    buffer->bufferRead(buf,size); //移除所有数据并且读到buf中
    printf("Total Size %ld,remain %ld\n",recvTotalSize,buffer->totalSize());
}
void debug(){
    base::Logger::setLogLevel(base::Logger::LogLevel::DEBUG);
}
int main(int argc,char**argv){


    InetAddress address(argv[1],atoi(argv[2]));
    
    EventLoop loop;
    TcpClient client(&loop,address);
    client.setWriteCompleteCallback(std::bind(&WriteComplete,_1));
    client.setConnectionCallback(std::bind(&NewConnection,_1));
    client.setMessageCallback(std::bind(&NewMessage,_1,_2));
    debug();
    client.start();
    loop.loop();




}
