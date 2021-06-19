
#include "Socket.h"

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
    
        sockets::close(sockfd_);

    }






}
