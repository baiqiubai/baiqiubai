
#include "Buffer.h"

#include <sys/uio.h>

namespace net {


size_t Buffer::readFd(int fd){

    
    char buf[65535];
    
    struct iovec iov[2];
    
    size_t size=writeAbleSpace();
    iov[0].iov_base=writeAbleStart();
    iov[0].iov_len=size;

    iov[1].iov_base=buf;
    iov[1].iov_len=sizeof(buf);

    size_t n=::readv(fd,iov,2);
    if(n > 0){
        
        if(n > size){//buf中也存在数据
            
            size_t readn=n-size;
            append(buf,readn);
            writeAbleIndex_+=size;
        }else {
            writeAbleIndex_+=n;
        }
    }else if(n <0 ){
        if(errno!=EINTR){
            perror("readv error");
        }else {
            perror("readv error");
        } 

    }else {
    
    
    }
    return n;

}







}
