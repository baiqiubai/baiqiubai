
#ifndef BUFFER_NET_H_
#define BUFFER_NET_H_

#include <iostream>
namespace net {
class BufferChain;
namespace detail{
    
    extern size_t kDefaultAllocSize;
    BufferChain *createNewBufferChain(size_t totalSize,
            size_t misalign=0);
}
class BufferChain{
    friend class Buffer;
public:
    BufferChain()
        :next_(NULL),
        bufferSize_(0),
        misalign_(0),
        off_(0){
        
    }
    ~BufferChain()=default;

    BufferChain(size_t allocSize,size_t misalign=0):
        next_(NULL),
        bufferSize_(allocSize),
        misalign_(misalign),
        off_(0){
        
        buffer_=reinterpret_cast<char*>(this+sizeof(BufferChain));    
    }

    void setBufferSize(int size){bufferSize_=size;}
    size_t bufferSize()const{return bufferSize_;}
    size_t misalign()const{return misalign_;}
    size_t off()const{return off_;}

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

class Buffer{

public:
    Buffer():
        first_(NULL),
        last_(NULL),
        lastWithData_(&first_),
        totalSize_(0)
    {
    }
    size_t totalSize()const{return totalSize_;}
    ~Buffer()=default;
    void BufferAdd(size_t dataSize,char *data);

    void foreachWithDataChain();
        
    size_t read(int sockfd);    
    
private:

    BufferChain* expandSingleChain(size_t datalen);
    void remove(char *data,size_t len);
    void copyOut(char *dataOut,size_t len);
    void Drain(size_t len);
    int afterChainisEmpty(BufferChain *chain);
    void freeAllChain(BufferChain *chain);
    void freeChain(BufferChain *chain);
    
        
    void insertBufferChain(BufferChain *chain);
    BufferChain *first_;//指向第一个bufferChain
    BufferChain *last_;//指向最后一个bufferchain
    BufferChain **lastWithData_;//指向最后一个有数据的BufferChain

    size_t totalSize_;
};





}


#endif 

