
#include "Connector.h"
#include "EventLoop.h"
#include "SocketOps.h"

#include <algorithm>
namespace net {
    
    size_t Connector::kMaxRetryDelay=500*100;

    Connector::Connector(EventLoop *loop,
            const InetAddress &addr)
    :loop_(loop),
    inetAddress_(addr),
    // connectSocket_(),
    // sockfd_(sockets::createNonBlockingSockfd()),
    connecting_(false),
    retryDelayms_(kInitretryDelay),
    state_(DisConnected){
   
        
    }

    
    void Connector::connect(){ //每次连接重新创建socket 否则出现ECONNABORTED 
       
        int sockfd=sockets::createNonBlockingSockfd();
        int ret=sockets::connect(sockfd,&inetAddress_);
        if(ret < 0){
            switch(errno){
                case 0:
                case EINTR:
                case EISCONN:
                case EINPROGRESS:
                    connecting(sockfd);
                    break;
                case ECONNREFUSED:
                case EAGAIN:
                    retry(sockfd);
                    break;
                default:
                    break; 
            
            }
            
        } 
    }
    void Connector::start(){
    
        loop_->runInLoop(std::bind(&Connector::startInLoop,this)); 
    }
    void Connector::startInLoop(){
    
        loop_->assertInLoop();
        if(state_==DisConnected){
            connect();
        }
    }
    int Connector::removeAndReset(){
        
        connectChannel_->disableAll();
        connectChannel_->remove();
        int sockfd=connectChannel_->fd();
        loop_->queueInLoop(std::bind(&Connector::resetChannel,this));
        return sockfd;

    }
    void Connector::resetChannel(){
    
        connectChannel_.reset();

    }
    void Connector::handlerWrite(){
    
        if(state_==Connecting){
            int sockfd=removeAndReset();
            int err=sockets::getSocketError(sockfd);
            if(err){
                  
                retry(sockfd);
            }else if(sockets::isSelfConnect(sockfd)) {

                retry(sockfd);
            
            }else {
            
                setState(Connected);
                if(newConnectionCallback_)newConnectionCallback_();
            }
        }
    }
    void Connector::connecting(int sockfd){
        setState(Connecting);
        connecting_=true;
        connectChannel_.reset(new Channel(loop_,sockfd));
        connectChannel_->setWriteCallback(std::bind(&Connector::handlerWrite,this));
        connectChannel_->enableWriting();


    }
    void Connector::retry(int sockfd){
        setState(DisConnected);
        sockets::close(sockfd);
        loop_->runAfter(std::bind(&Connector::startInLoop,this),retryDelayms_/1000.0) ;
        retryDelayms_=std::min(2*retryDelayms_,kMaxRetryDelay);
    
    }
    
}

