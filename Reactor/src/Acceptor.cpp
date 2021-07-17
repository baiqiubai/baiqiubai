
#include "Acceptor.h"
#include "Channel.h"
#include "SocketOps.h"

#include "Logger.h"
#include "LogStream.h"
using namespace base;
namespace net {

    Acceptor::Acceptor(EventLoop *loop,const InetAddress &addr)
    :loop_(loop),
    listening_(false),
    inetAddress_(addr),
    socket_(),
    socketChannel_(new Channel(loop_,socket_.fd())){
            
        socketChannel_->setReadCallback(std::bind(&Acceptor::handleRead,this));    
        socket_.setResuePort(true);
    }

    void Acceptor::bind(InetAddress *address){
    
        sockets::bind(socket_.fd(),address);
    }
    void Acceptor::listen(){
    
        LOG_DEBUG<<"Acceptor::listen fd:"<<socket_.fd();
        listening_=true;
    
    
        bind(&inetAddress_);
        sockets::listen(socket_.fd());
        socketChannel_->enableReading();
        

    }

    void Acceptor::handleRead(){
    
        int connfd=sockets::accept(socket_.fd());   
        if(connfd <0 ){

            sockets::close(connfd);
        }else{ 
            LOG_DEBUG<<"Acceptor::handleRead Accept connfd:"<<connfd;
            sockets::setNonBlocking(connfd);
            if(newConnectionCallback_)newConnectionCallback_(connfd);
        }
    }






}
