
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
    
        tcpConnectionChannel_->setReadCallback(std::bind(&TcpConnection::handlerRead,this));
        tcpConnectionChannel_->setCloseCallback(std::bind(&TcpConnection::handlerClose,this));
        tcpConnectionChannel_->setErrorCallback(std::bind(&TcpConnection::handlerError,this));
        tcpConnectionChannel_->setWriteCallback(std::bind(&TcpConnection::handlerWrite,this));
    }
  

    TcpConnection::~TcpConnection(){
   
    
        LOG_INFO<<"TcpConnection destructor";
    }

    void TcpConnection::connectEstablished(){
    
        setState(Connected);
        tcpConnectionChannel_->enableReading();
        if(connectionCallback_)connectionCallback_(shared_from_this());
    }
    void TcpConnection::removeConnectionInPoller(){
        
        tcpConnectionChannel_->remove();
    }
    
    void TcpConnection::sendInLoop(const std::string &message){
        size_t len=message.size();
        ssize_t nwrite;
        size_t remain;
        if(outputBuffer_.readAbleSpace()==0){//没有还未读走的数据
            nwrite=::write(tcpSocket_.fd(),message.data(),len);
            if(nwrite >= 0){
                remain=len-nwrite;
                if(remain ==0 ){
                    if(writeCompleteCallback_)
                        loop_->queueInLoop(std::bind(writeCompleteCallback_,shared_from_this()));
                }
            }else {
                if(errno!=EAGAIN)
                    LOG_ERROR<<"send error";
            }
        } 
        if(nwrite < len){//一次没有写完加入output下次继续写
            size_t oldlen=outputBuffer_.readAbleSpace();
            if(oldlen + remain >=highWaterMark_ && highWaterMarkCallback_){
                loop_->queueInLoop(std::bind(highWaterMarkCallback_,shared_from_this()));
            }
            outputBuffer_.append(message.data()+nwrite,len-nwrite);
            if(!tcpConnectionChannel_->isWriting()){
                tcpConnectionChannel_->enableWriting();
            }
        }

    }
    void TcpConnection::send(const std::string &message){
    
        
        loop_->runInLoop(std::bind(&TcpConnection::sendInLoop,this,message));

    }

    void TcpConnection::shutdown(){
    
        loop_->runInLoop(std::bind(&TcpConnection::shutdownInLoop,
                    this));
    
    }
    void TcpConnection::shutdownInLoop(){

        if(state_==Connected){
            setState(DisConnecting);
            tcpSocket_.shutdown();
        }
    
    }
    void TcpConnection::handlerWrite(){
    
        
        if(tcpConnectionChannel_->isWriting()){
        
            ssize_t n=::write(tcpSocket_.fd(),outputBuffer_.readAbleStart(),
                    outputBuffer_.readAbleSpace()); 

            if( n>0){
                outputBuffer_.retireve(n);
                if(outputBuffer_.readAbleSpace()==0){ //停止关注事件
                    tcpConnectionChannel_->disableWrite();              
                    if(writeCompleteCallback_)loop_->queueInLoop(std::bind(writeCompleteCallback_,shared_from_this()));
                }
            }else {
                
                LOG_ERROR<<"handlerWrite error";
            }
        }
    
    
    
    
    
    }
    void TcpConnection::handlerRead(){
    
        size_t n=inputBuffer_.readFd(tcpSocket_.fd());

        if(n > 0 ){
            messageCallback_(shared_from_this(),&inputBuffer_);
        }else if(n==0){
            handlerClose();
        }else {
            
            handlerError();
        }
        
        
    }

    void TcpConnection::handlerClose(){
        
            setState(DisConnected);
            tcpConnectionChannel_->disableAll(); //必须在此时取消对所有事件的关注 LT 模式否则多次触发handlerClose
            closeCallback_(shared_from_this());
    }
        
    
    void TcpConnection::handlerError(){
    
        
        LOG_ERROR<<"readFd error";
    }



}
