
#include "TcpServer.h"
#include "EventLoop.h"
#include "SocketOps.h"
#include "Logger.h"
#include "LogStream.h"

#include <assert.h>
using namespace base;
namespace net {

    TcpServer::TcpServer(EventLoop *loop,
            const InetAddress &address,
            std::string name)
    :loop_(loop),
    acceptor_(new Acceptor(loop_,address)),
    name_(std::move(name)),
    threadPool_(new EventLoopThreadPool(loop_))
    {
    
        acceptor_->setNewConnectionCallback(std::bind(&TcpServer::OnConnection,this,_1));
        
    }
    
    void TcpServer::setThreadNum(int threadNum){
    
        threadPool_->setThreadNum(threadNum);
        LOG_DEBUG<<"Base Loop Tid:"<<loop_->loopTid();
    }
    void TcpServer::start(){
        
        threadPool_->start();
        acceptor_->listen();
    }
    
    void TcpServer::closeConnection(const TcpConnectionPtr &conn){  
        
            loop_->runInLoop(std::bind(&TcpServer::removeConnection,this,conn)); //从TcpConnection的IO线程转移至TcpServer的loop线程
        
    }
    void TcpServer::removeConnection(const TcpConnectionPtr&conn){
        
        LOG_DEBUG<<"removeConnection Name:"<<conn->name();  
        size_t n=connections_.erase(conn->name());
        assert(n==1);

        EventLoop *ioLoop=conn->getLoop();
        //不能直接在handlerEvent remove Channel 采用queueInLoop 
    
        ioLoop->queueInLoop(std::bind(&TcpConnection::removeConnectionInPoller,conn));
    
    }
    void TcpServer::OnConnection(int sockfd){
    
        
        
        char buf[32]={0};
        snprintf(buf,sizeof(buf),"%d",sockfd);
        std::string name=name_;
        name+=buf;
        

        EventLoop *ioLoop=threadPool_->getNextLoop(); //每次取一个Loop
        InetAddress localAddress(acceptor_->inetAddress());
        
        InetAddress peerAddress;
        sockets::getpeername(sockfd,&peerAddress);
        

        LOG_DEBUG<<"TcpConnection Name: "<<name<<" IO LOOP "<<ioLoop->loopTid( );
        std::shared_ptr<TcpConnection> conn(new TcpConnection(ioLoop,localAddress,
                    peerAddress,sockfd,name));
    

        conn->setHighWaterMarkCallback(highWaterMarkCallback_);
        conn->setWriteCompleteCallback(writeCompleteCallback_);

        conn->setCloseConnectionCallback(std::bind(&TcpServer::closeConnection,this,_1));
        conn->setConnectionCallback(connectionCallback_);
        conn->setMessageCallback(messageCallback_);
        connections_[name]=conn;
        

        ioLoop->runInLoop(std::bind(&TcpConnection::connectEstablished,conn));

    }






}
