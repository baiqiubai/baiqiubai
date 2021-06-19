
#ifndef SOCKETOPS_NET_H_
#define SOCKETOPS_NET_H_

#include "InetAddress.h"

#include <arpa/inet.h>
namespace net {

namespace sockets{

const struct sockaddr * sockaddr_cast(const struct sockaddr_in *addr);
struct sockaddr * sockaddr_cast(struct sockaddr_in *addr);

int createNonBlockingSockfd();
void close(int sockfd);
void listen(int sockfd);
void bind(int sockfd,InetAddress *address);
int accept(int sockfd,InetAddress *address);
int accept(int sockfd);
int connect(int sockfd,InetAddress *address);
void shutdown(int sockfd);


void inet_pton(int af, const char *src, void *dst);
void inet_ntop(int af, const void *src,char *dst, socklen_t size);



uint32_t htonl(uint32_t hostlong);
uint16_t htons(uint16_t hostshort);
uint32_t ntohl(uint32_t netlong);
uint16_t ntohs(uint16_t netshort);
  

void getpeername(int sockfd, InetAddress *address);
void setResuePort(int sockfd);
int  getSocketError(int sockfd);
bool isSelfConnect(int sockfd);
}





}


#endif 
