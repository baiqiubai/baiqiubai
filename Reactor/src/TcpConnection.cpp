
#include "TcpConnection.h"
#include "EventLoop.h"
#include "Logger.h"
#include "LogStream.h"


#include <unistd.h>

using namespace base;
namespace net {

    TcpConnection::TcpConnection(EventLoop *loop,
            const InetAddress &localAddress,
            const InetAddress &peerAddress,
            int sockfd,
            std::string name)
    :loop_(loop),
    localAddress_(localAddress),
    peerAddress_(peerAddress),
    tcpSocket_(sockfd),
    tcpConnectionChannel_(new Channel(loop_,tcpSocket_.fd())),
    connName_(std::move(name)),
    state_(Connecting),
    highWaterMark_(64*1024*1024){
    

        tcpConnectionChannel_->setReadCallback(std::bind(&TcpConnection::handleRead,this));
        tcpConnectionChannel_->setCloseCallback(std::bind(&TcpConnection::handleClose,this));
        tcpConnectionChannel_->setErrorCallback(std::bind(&TcpConnection::handleError,this));
        tcpConnectionChannel_->setWriteCallback(std::bind(&TcpConnection::handleWrite,this));
    }
  

    TcpConnection::~TcpConnection(){
   
    
        LOG_INFO<<"TcpConnection::~TcpConnection Name:"<<connName_;
    }

    void TcpConnection::connectEstablished(){
        LOG_DEBUG<<"TcpConnection::connectEstablished";
        setState(Connected);
        tcpConnectionChannel_->enableReading();
        if(connectionCallback_)connectionCallback_(shared_from_this());
    }
    void TcpConnection::removeConnectionInPoller(){
        
        tcpConnectionChannel_->remove();
    }
   void TcpConnection::send(const char*message,size_t datalen){
        

        loop_->runInLoop(std::bind(&TcpConnection::sendInLoop,this,message,datalen));
    }
    void TcpConnection::sendInLoop(const char*message,size_t datalen){
   
        if(!tcpConnectionChannel_->isWriting()&&outputBuffer_.totalSize()==0){
            outputBuffer_.bufferAdd(datalen,message);
            size_t remain=datalen; 
            ssize_t nwrite=outputBuffer_.writeIovec(tcpSocket_.fd(),remain);
            
            if(static_cast<size_t>(nwrite)<remain)
                tcpConnectionChannel_->enableWriting(); //一次没有写完/ 
            else 
                if(writeCompleteCallback_)loop_->runInLoop(std::bind(writeCompleteCallback_,shared_from_this()));

            LOG_DEBUG<<"TcpConnection::sendInLoop Write:"<<static_cast<int>(nwrite)<<" Bytes ";
            if(nwrite>0)outputBuffer_.bufferDrain(nwrite);
            
        
        }else {
             outputBuffer_.bufferAdd(datalen,message);
        }
    }

    void TcpConnection::shutdown(){
    

        loop_->runInLoop(std::bind(&TcpConnection::shutdownInLoop,
                    this));
    
    }
    void TcpConnection::shutdownInLoop(){
    
        LOG_DEBUG<<"TcpConnection::shutdownInLoop ";
        if(state_==Connected){
            setState(DisConnecting);
            tcpSocket_.shutdown();
        }
    
    }
    void TcpConnection::handleWrite(){ //从outputBuffer中向socket中写数据
    
        
         
        if(tcpConnectionChannel_->isWriting()) {
            
            if(outputBuffer_.totalSize())   {
                size_t remain=outputBuffer_.totalSize();
                
                ssize_t n=outputBuffer_.writeIovec(tcpSocket_.fd(),remain);
                
                if(static_cast<size_t>(n)==remain){ //写完所有数据 停止关注事件    
                    tcpConnectionChannel_->disableWrite();
                    if(writeCompleteCallback_)loop_->runInLoop(std::bind(writeCompleteCallback_,shared_from_this()));
                }
                if(n>0)outputBuffer_.bufferDrain(n);
                LOG_DEBUG<<"TcpConnection::handleWrite Write:"<<static_cast<int>(n)<<" Bytes";
            } 

        }     

    }
  /*  void TcpConnection::handleWrite(){
    
        
        if(tcpConnectionChannel_->isWriting()){
        
            ssize_t n=::write(tcpSocket_.fd(),outputBuffer_.readAbleStart(),
                    outputBuffer_.readAbleSpace()); 

            if(n>=0){
                outputBuffer_.retireve(n);
                if(outputBuffer_.readAbleSpace()==0){ //停止关注事件
                    tcpConnectionChannel_->disableWrite();              
                    if(writeCompleteCallback_)loop_->queueInLoop(std::bind(writeCompleteCallback_,shared_from_this()));
                }
            }else {
                LOG_ERROR<<"handleWrite error";
            }
        }
    
    
    
    
    
    }*/
    void TcpConnection::handleRead(){
    
        // size_t n=inputBuffer_.readFd(tcpSocket_.fd());
        size_t n=inputBuffer_.bufferRead(tcpSocket_.fd());
        LOG_DEBUG<<"TcpConnection::handleRead Read:"<<static_cast<int>(n)<<" Bytes ";
        if(n > 0 ){
            messageCallback_(shared_from_this(),&inputBuffer_);
        }else if(n==0){
            handleClose();
        }else {
            
            handleError();
        }
        
        
    }

    void TcpConnection::handleClose(){
            LOG_DEBUG<<"TcpConnection::handleClose ";
            setState(DisConnected);
            tcpConnectionChannel_->disableAll(); //必须在此时取消对所有事件的关注 LT 模式否则多次触发handleClose
            closeCallback_(shared_from_this());
    }
        
    
    void TcpConnection::handleError(){
    
         
        LOG_ERROR<<"readFd error";
    }



}
