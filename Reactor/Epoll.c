
#include <stdio.h>
#include <sys/epoll.h>

int main(){
    
    printf("%d\n",EPOLL_CTL_ADD);
    printf("%d\n",EPOLL_CTL_MOD);
    printf("%d\n",EPOLL_CTL_DEL);
}

