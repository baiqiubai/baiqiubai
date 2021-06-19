
#include "Acceptor.h"
#include "Channel.h"
namespace net {

    Acceptor::Acceptor(EventLoop *loop,const InetAddress &addr)
    :loop_(loop),
    listening_(false),
    inetAddress_(addr),
    socket_(),
    socketChannel_(new Channel(loop_,socket_.fd())){
            
        socketChannel_->setReadCallback(std::bind(&Acceptor::handlerRead,this));    
        socket_.setResuePort(true);
    }

    void Acceptor::bind(InetAddress *address){
    
        sockets::bind(socket_.fd(),address);
    }
    void Acceptor::listen(){
    
        listening_=true;
    
    
        bind(&inetAddress_);
        sockets::listen(socket_.fd());
        socketChannel_->enableReading();
        

    }

    void Acceptor::handlerRead(){
    
        int connfd=sockets::accept(socket_.fd());   

        if(connfd <0 ){

            sockets::close(connfd);
        }else 
            if(newConnectionCallback_)newConnectionCallback_(connfd);
    }






}
