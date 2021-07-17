

#include "SocketOps.h"

#include <fcntl.h>
#include <cstring>
#include <unistd.h>
#include <errno.h>
#include <cstdio>
namespace net {


namespace sockets{

const struct sockaddr * sockaddr_cast(const struct sockaddr_in *addr){


    return static_cast<const struct sockaddr*>(static_cast<const void*>(
                addr));
}
struct sockaddr * sockaddr_cast(struct sockaddr_in *addr){

    return static_cast<struct sockaddr*>(static_cast<void*>(addr));
}


int createNonBlockingSockfd(){
    int sockfd=::socket(AF_INET,SOCK_STREAM|SOCK_NONBLOCK|SOCK_CLOEXEC,0);
    if(sockfd < 0){
        perror("createNonBlockingSockfd error");
    }
    return sockfd;


}
void close(int sockfd){
    
    if(::close(sockfd)){
        perror("close sockfd error");
    }
        
}
void listen(int sockfd){
    if(::listen(sockfd,5)){
    
    
    }

}
void bind(int sockfd,InetAddress *address){

    socklen_t len=sizeof(struct sockaddr);
    if(::bind(sockfd,sockaddr_cast(&address->sockAddrRef()),len)){
        
        perror("bind error");
    }

}

int accept(int sockfd,InetAddress *address){

    int connfd;
    socklen_t len=sizeof(struct sockaddr_in);
    if((connfd=::accept(sockfd,sockaddr_cast(&address->sockAddrRef()),&len))==-1){
        
        perror("accept error");
    }
    return connfd;

}
int accept(int sockfd){

    return ::accept(sockfd,NULL,NULL);

}
int connect(int sockfd,InetAddress *address){

    int ret;
    socklen_t len=sizeof(struct sockaddr_in);
    ret=::connect(sockfd,sockaddr_cast(&address->sockAddrRef()),len);
    return ret;

}
void shutdown(int sockfd){

    if(::shutdown(sockfd,SHUT_RD))    
        perror("shutdown error");
}
void inet_pton(int af, const char *src, void *dst){

    if(::inet_pton(af,src,dst)!=1){
        perror("inet_pton error");
    
    }

}
void inet_ntop(int af, const void *src,char *dst, socklen_t size){

    if(::inet_ntop(AF_INET,src,dst,size)==NULL){
        
        perror("inet_ntop error");
    
    }

}
void setNonBlocking(int sockfd){
    int oldOp=fcntl(sockfd,F_GETFL);
    int newOp=oldOp|O_NONBLOCK;
    fcntl(sockfd,F_SETFL,newOp);    
}

uint32_t htonl(uint32_t hostlong){
    
    return ::htonl(hostlong);
}
uint16_t htons(uint16_t hostshort){
    
    return ::htons(hostshort);
}
uint32_t ntohl(uint32_t netlong){

    return ::ntohl(netlong);
}
uint16_t ntohs(uint16_t netshort){

    return ::ntohs(netshort);
}



void getpeername(int sockfd,InetAddress *address){


    socklen_t len=sizeof(struct sockaddr_in);

    if(::getpeername(sockfd,sockaddr_cast(&address->sockAddrRef()),&len)==-1){
        perror("getpeername error");
    }


}

void setResuePort(int sockfd){

    int resue=1;
    if(::setsockopt(sockfd,SOL_SOCKET,SO_REUSEPORT,&resue,sizeof(resue))){
    
        perror("setResuePort error");
    }

}
int getSocketError(int sockfd){

    int ret;
    int one;
    socklen_t len=sizeof(one);
    if((ret=::getsockopt(sockfd,SOL_SOCKET,SO_ERROR,&one,&len))<0){
   
        return errno;
        // perror("getSockeError error");
    }else {
        return one;
    }

}
bool isSelfConnect(int sockfd){

    struct sockaddr_in local;
    struct sockaddr_in peer;
    memset(&local,'\0',sizeof(local));
    memset(&peer,'\0',sizeof(peer));
    socklen_t locallen=sizeof(local);
    socklen_t peerlen=sizeof(peer);
    ::getsockname(sockfd,sockaddr_cast(&local),&locallen);
    ::getpeername(sockfd,sockaddr_cast(&peer),&peerlen);

    return local.sin_port==peer.sin_port 
        && local.sin_addr.s_addr==peer.sin_addr.s_addr;
    
}

}








}
