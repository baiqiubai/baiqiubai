
#ifndef BUFFER_NET_H_
#define BUFFER_NET_H_
#include "noncopyable.h"

#include <sys/uio.h>
#include <functional>
#include <iostream>
#include <cstring>
namespace net {
class BufferChain;
namespace detail{
    
    extern size_t kDefaultAllocSize;
    BufferChain *createNewBufferChain(size_t totalSize,
            size_t misalign=0);

    size_t getSockfdBytes(int sockfd); //获取fd中字节
}
class BufferChain{
    friend class Buffer;
public:
    BufferChain()
        :next_(NULL),
        bufferSize_(0),
        misalign_(0),
        off_(0),
        buffer_(NULL)
    {
        
    }
    ~BufferChain(){
     
        delete []buffer_;
        buffer_=NULL;
    }
    
    BufferChain(const BufferChain& chain){
        size_t size=chain.bufferSize();
        buffer_=new char[size];
        ::memcpy(buffer_,chain.buffer_+chain.misalign_+chain.off_,chain.off_);
    }
    BufferChain& operator=(BufferChain &chain){
        
        return *this;
    }
    BufferChain(size_t allocSize,size_t misalign=0):
        next_(NULL),
        bufferSize_(allocSize),
        misalign_(misalign),
        off_(0),
        buffer_(NULL)
    {
    
        buffer_=new char[allocSize];
    }

    size_t bufferSize()const{return bufferSize_;}
    size_t misalign()const{return misalign_;}
    size_t off()const{return off_;}

    void setOff(size_t off){off_=off;}
    char *buffer(){return buffer_;}

    bool shouldRealign(size_t dataSize);
    
    void Realign();
private:
    
    BufferChain *next_;

    size_t bufferSize_;//分配的BufferChain总大小
    size_t misalign_;//偏移不用的
    
    size_t off_; //写了多少字节

    char *buffer_;//指向分配的空间
};

class Buffer:public base::noncopyable{

public:
    Buffer():
        first_(NULL),
        last_(NULL),
        lastWithData_(&first_),
        totalSize_(0)
    {
    }
    
    using BufferCallback=std::function<void()>;
    size_t totalSize()const{return totalSize_;}
    ~Buffer()=default;
    void bufferAdd(size_t dataSize,const char *data);

    void foreachWithDataChain();
    
    void bufferRead(char *buf,size_t datalen); //用户使用的函数
    //从buffer中remove datalen字节 


    void bufferInsertChain(BufferChain *chain);
    BufferChain *first(){return first_;}
    BufferChain *last(){return last_;}
    BufferChain **lastWithData(){return lastWithData_;}

    size_t bufferRead(int sockfd);  //用户不能使用
    ssize_t writeIovec(int sockfd,size_t howmuch);
    void bufferDrain(size_t len);


private:
    void  bufferExpandFast(size_t n,int IovecNum);
    //利用iovec快速预留空间 
    //
    int bufferSetUpVecs(size_t n,struct iovec *vecs,int numIovec);

    int bufferCopyout(char *dataOut,size_t len);
    int  afterChainisEmpty(BufferChain *chain);
    void deleteAllChain(); 
    void deleteAfterChain(BufferChain *chain);
    
        
    BufferChain *first_;//指向第一个bufferChain
    BufferChain *last_;//指向最后一个bufferchain
    BufferChain **lastWithData_;//指向最后一个有数据的BufferChain

    size_t totalSize_;

    // BufferCallback bufferCallback_;
};





}


#endif 
