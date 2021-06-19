
#ifndef SOCKET_NET_H_
#define SOCKET_NET_H_

#include "SocketOps.h"
#include "InetAddress.h"
namespace net {

class Socket{
    public:
        Socket(int fd):sockfd_(fd){}
        Socket();

        int fd()const{return sockfd_;}
        void listen();
        
        void shutdown();

        void setResuePort(bool open=true);
        ~Socket();

    private:
        const int sockfd_;
};





}

#endif
