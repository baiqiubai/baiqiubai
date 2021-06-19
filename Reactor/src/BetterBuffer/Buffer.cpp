
#include "Buffer.h"

#include <unistd.h>
#include <assert.h>
#include <cstring>
namespace net {


namespace detail{
    size_t kDefaultAllocSize=512;
    BufferChain *createNewBufferChain(size_t totalSize,
            size_t misalign){
    //假如totalSize=500 则toAlloc =512; bufferSize 为512-sizeof(BufferChain)
            size_t size=totalSize+sizeof(BufferChain); 
            
            size_t toAlloc=detail::kDefaultAllocSize;
            if(size > toAlloc){
                toAlloc=size;
            }else {
                while(toAlloc <size )
                    toAlloc<<=1;
            }
        
            BufferChain *buffer=new BufferChain(toAlloc,misalign);
            buffer->setBufferSize(toAlloc-sizeof(BufferChain));
            return buffer;
    }


}

bool BufferChain::shouldRealign(size_t dataSize){
    
    return bufferSize_-off_ > dataSize; 
}
void BufferChain::Realign(){

    ::memmove(buffer_+misalign_,buffer_+misalign_+off_,off_);
    misalign_=0;
}
void Buffer::foreachWithDataChain(){
    
    int cnt=0; 
    for(BufferChain *chain=first_;chain!=NULL;chain=chain->next_){
        printf("cnt %d bytes %ld buf \n",cnt++,chain->off_);
    }

}
void Buffer::freeChain(BufferChain *chain){


    delete chain;
}
void Buffer::freeAllChain(BufferChain *chain){

    for(;chain;chain=chain->next_){
        freeChain(chain);
    }

}
int Buffer::Buffer::afterChainisEmpty(BufferChain *chain){

    for(;chain;chain=chain->next_)
        if(chain->off())return 0;
    return 1;

}
void Buffer::insertBufferChain(BufferChain *chain){

        if(*lastWithData_==NULL)
            first_=last_=chain;
        else {
        
            struct BufferChain **ch=lastWithData_;

            while((*ch) && (*ch)->off())
                ch=&(*ch)->next_;
            
            if(*ch==NULL){
                last_->next_=chain;
                if(chain->off()){ //有数据
                        lastWithData_=&last_->next_;//更新
                }
            }else {
                assert(afterChainisEmpty(*ch));
                freeAllChain(*ch);
                *ch=chain;
            }
            last_=chain;
        
 
        
        }

}

void Buffer::remove(char *data,size_t len){


    copyOut(data,len);
    
    Drain(len);
}
void Buffer::copyOut(char *dataOut,size_t len){
    
    BufferChain *chain=first_;

    size_t size=len;
    if(totalSize_ > size){
        
        size=totalSize_;
    }

    while(size && size>=chain->off()){
   
        ::memcpy(dataOut,chain->buffer_+chain->off_+chain->misalign_,chain->off_);

        dataOut+=chain->off_;  
        
        size-=chain->off_;
    
        chain=chain->next_;
    }

    if(size) {
        ::memcpy(dataOut,chain->buffer_+chain->off_+chain->misalign_,size);
    }


}
void Buffer::Drain(size_t len){


    BufferChain *chain=first_;
    if(totalSize_ <= len){ //全部移除
        freeAllChain(chain);
    }else { //总大小大于len
    
        totalSize_-=len;
        for(;len >=chain->off();chain=chain->next_){
            len-=chain->off_;
            
            if(*lastWithData_==chain){//最后一个有数据的chain
                lastWithData_=&first_;
            }
    
            
            if(&chain->next_==lastWithData_){//倒数第二个chain
                lastWithData_=&first_;
            }
            freeChain(chain);
        }
        if(chain){
        
            chain->misalign_+=len;//这个chain大小大于len
            chain->off_-=len;
        }
            
    }


}

#define CHAIN_SPACE_LEN(ch) ((ch)->bufferSize_-((ch)->misalign_+(ch)->off_)) 
//预留空间使得chain能容纳下datalen
BufferChain *Buffer::expandSingleChain(size_t datalen){

    assert(datalen >0); 
    BufferChain **ch=lastWithData_;
    
    BufferChain *result;
    while((*ch) && CHAIN_SPACE_LEN(*ch)==0) //找到一个剩余空间不为0的chain
        ch=&(*ch)->next_;


    BufferChain *chain=*ch;

    if(*ch==NULL){//没有找到 直接插入新的
        goto out;
    }else {
        if(CHAIN_SPACE_LEN(*ch)>datalen){
            result=*ch;
            goto res;
        }
        if(CHAIN_SPACE_LEN(*ch) <datalen){//剩余空间小于datalen
            if((*ch)->shouldRealign(datalen)){//调整misalign可以容纳下
                (*ch)->Realign();
                result=(*ch);
                goto res;
            }
        }

        //剩余空间小于总空间1/8 或者写入的off大于4096 不移动chain 重新分配
        if(CHAIN_SPACE_LEN(*ch) < (*ch)->bufferSize_ / 8 ||
                (*ch)->off_ > 4096){
            
            if((*ch)->next_ && CHAIN_SPACE_LEN((*ch)->next_)>datalen){
                result=(*ch)->next_;
                goto res;
            }else 
                goto out;
        
        }else {

            size_t size=datalen+chain->bufferSize();
            BufferChain *temp=detail::createNewBufferChain(size);

            ::memcpy(temp->buffer_+temp->off_+temp->misalign_,
                    chain->buffer_+chain->misalign_+chain->off_,
                    chain->off_);

            temp->off_+=chain->off_;
            result=temp;

            temp->next_=chain->next_;
            freeChain(chain);
            goto res;
                    
        }
    
    }

out:
    chain=detail::createNewBufferChain(datalen);
    result=chain;

res:
    return result;
}

size_t Buffer::read(int sockfd){

    

    char *p;

    size_t n=::read(sockfd,p,datalen);
        
}
void Buffer::BufferAdd(
        size_t dataSize,
        char *data){

       size_t remain; 
       BufferChain *chain; 
       if(*lastWithData_==NULL)
           chain=last_;
       else 
           chain=*lastWithData_;

       if(chain==NULL){
            
           chain=detail::createNewBufferChain(dataSize,0);
           insertBufferChain(chain);
       }
            
       //剩余大小
       remain=chain->bufferSize()-chain->off()-chain->misalign(); 
       if(remain >= dataSize){//直接拷贝
        
            ::memcpy(chain->buffer_+chain->off_+chain->misalign_,data,dataSize);    
            
            chain->off_+=dataSize;
            
            totalSize_+=dataSize;

       }else if(chain->shouldRealign(dataSize)){ //调整mislign还能放下数据
                
            chain->Realign();
            ::memcpy(chain->buffer_+chain->off_,
                    data,dataSize);

            chain->off_+=dataSize;
            totalSize_+=dataSize;
       }else {//需要新的chain
            size_t toAlloc=chain->bufferSize();
            
            if(toAlloc < 2048)
                toAlloc<<=1;
            if(toAlloc < dataSize)
                toAlloc=dataSize;
            BufferChain *newChain=detail::createNewBufferChain(toAlloc,0); 
            
            size_t chainlen=chain->bufferSize()-chain->off()-chain->misalign();
            ::memcpy(chain->buffer_+chain->misalign_+chain->off_, //chain放满
                    data,chainlen);
            
            chain->off_+=chainlen;
            totalSize_+=chainlen;
            dataSize-=chainlen;
            data+=chainlen;

            ::memcpy(newChain->buffer_+newChain->off_+newChain->misalign_,
                    data,dataSize);

            newChain->off_+=dataSize;
            insertBufferChain(newChain);
            totalSize_+=dataSize;
       } 
}







}
