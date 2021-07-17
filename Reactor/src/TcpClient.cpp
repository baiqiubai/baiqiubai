
#include "TcpClient.h"
#include "Logger.h"
#include "LogStream.h"
#include "TcpConnection.h"
using namespace base;
namespace  net {

    TcpClient::TcpClient(EventLoop *loop,
            const InetAddress &servaddress,
            std::string name)
    :peerAddress_(servaddress),
    loop_(loop),
    connector_(new Connector(loop_,peerAddress_)),
    retry_(false),
    name_(std::move(name)){
    
        connector_->setNewConnectionCallback(std::bind(&TcpClient::OnConnection,
                    this,_1));
    

    }

    
    TcpClient::~TcpClient(){ 
        
        LOG_DEBUG<<"TcpClient::~TcpClient ";

        if(connPtr_) {//连接上


            loop_->runInLoop(std::bind(&TcpConnection::removeConnectionInPoller,connPtr_)); 
        }else {//没有连接上
        
        }
    
    }  
   
    void TcpClient::start(){
    
        connector_->start();
    }
    void TcpClient::stop(){
        connector_->stop();
    }
    void TcpClient::removeConnection(const TcpConnectionPtr &conn){
        
        
        if(!retry_)
           loop_->queueInLoop(std::bind(&TcpConnection::removeConnectionInPoller,conn));
        else
            connector_->restart();
    
    }
    void TcpClient::OnConnection(int sockfd){
    
       
        char buf[32]={0};
        
        snprintf(buf,sizeof(buf),"%d",sockfd);
        std::string connName(name_);
        connName+=buf;

        InetAddress localAddress;
        sockets::getpeername(sockfd,&localAddress);

        LOG_DEBUG<<"TcpClient::OnConnection New TcpConnection Name:"<<connName; 
        
        connPtr_.reset(new TcpConnection(loop_,
                localAddress,peerAddress_,sockfd,connName));
        connPtr_->setWriteCompleteCallback(writeCompleteCallback_);
        connPtr_->setCloseConnectionCallback(std::bind(&TcpClient::removeConnection,this,_1));
        connPtr_->setConnectionCallback(connectionCallback_);
        connPtr_->setMessageCallback(messageCallback_);
    


        loop_->runInLoop(std::bind(&TcpConnection::connectEstablished,connPtr_));
    }

    


}
