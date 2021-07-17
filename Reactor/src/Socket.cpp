
#include "Socket.h"
#include "Logger.h"
#include "LogStream.h"

using namespace base;
namespace net {

    Socket::Socket():sockfd_(sockets::createNonBlockingSockfd()){
    
    
    }
    
    void Socket::shutdown(){
    
        sockets::shutdown(sockfd_);
    }

    void Socket::setResuePort(bool open){
        if(open){
            sockets::setResuePort(sockfd_);
        }

    }
    void Socket::listen(){
    
   
        sockets::listen(sockfd_);
    }
    Socket::~Socket(){
        LOG_DEBUG<<"Socket::~Socket sockfd_:"<<sockfd_;    
        sockets::close(sockfd_);

    }






}
