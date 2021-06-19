

#include "InetAddress.h"
#include "SocketOps.h"

#include <cstring>
namespace net {

    InetAddress::InetAddress(const char*ip,int port){
        
        memset(&sockAddr_,'\0',sizeof(sockAddr_));
        sockAddr_.sin_family=AF_INET;

        sockets::inet_pton(AF_INET,ip,&sockAddr_.sin_addr); 
    
        if(!port){
            sockAddr_.sin_port=sockets::htons(INADDR_ANY);
        }else {
        
            sockAddr_.sin_port=sockets::htons(port);
        }
    }

    std::string InetAddress::ip()const {
       
        char buf[32]={0};
        socklen_t len=sizeof(buf);
        sockets::inet_ntop(AF_INET,&sockAddr_.sin_addr.s_addr,buf,len);
        std::string result(buf);
        return result;
    }

    uint16_t InetAddress::port()const{
        
        uint16_t port;
        port=sockets::ntohs(sockAddr_.sin_port);
        return port;
    
    }







}
