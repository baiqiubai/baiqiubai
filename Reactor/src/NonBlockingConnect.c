
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>


int main(int argc,char **argv){


    struct sockaddr_in serv;
    serv.sin_family=AF_INET;
    inet_pton(AF_INET,argv[1],&serv.sin_addr);
    serv.sin_port=htons(atoi(argv[2]));

    int i=0;
    int sockfd=socket(AF_INET,SOCK_STREAM|SOCK_CLOEXEC|SOCK_NONBLOCK,0);
again:
    if(connect(sockfd,(struct sockaddr*)&serv,sizeof(serv))<0){
        if(errno!=EINPROGRESS&&errno!=ECONNREFUSED){
            printf("errno =%d\n",errno);
            perror("connect error");
        }else {
            printf("errno %d\n",errno);
            sleep(i+1); 
            ++i;
            goto again;
        }
    
    }

   char buf[190]={0}; 
   read(STDIN_FILENO,buf,sizeof(buf));
   write(sockfd,buf,sizeof(buf));







}

