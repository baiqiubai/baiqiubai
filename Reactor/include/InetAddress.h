
#ifndef INETADDRESS_NET_H_
#define INETADDRESS_NET_H_

#include <sys/socket.h>
#include <arpa/inet.h>

#include <iostream>
namespace net {


class InetAddress{
    public:
        InetAddress(){}
        explicit InetAddress(const char*ip,int port=0);
        
        std::string ip()const ;
        uint16_t port()const ;
        struct sockaddr_in& sockAddrRef(){return sockAddr_;}
        struct sockaddr_in sockAddr()const{return sockAddr_;}  
    private:

        struct sockaddr_in sockAddr_;
};




}


#endif 
