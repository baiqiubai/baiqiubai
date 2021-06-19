
#ifndef BUFFER_NET_H_
#define BUFFER_NET_H_

#include <iostream>
#include <cstring>
#include <vector>
namespace net {

const int kDefaultSize=1024;
#define BUFFERSIZE sizeof(Buffer)
class Buffer{
    public:
        Buffer(size_t dataSize=kDefaultSize)
        :readAbleIndex_(0),
        writeAbleIndex_(0)
    {
         buffer_.reserve(dataSize);
    }
    

    size_t readAbleIndex()const{return readAbleIndex_;}    
    size_t writeAbleIndex()const{return writeAbleIndex_;}

    size_t readAbleSpace(){return writeAbleIndex_-readAbleIndex_;}
    size_t writeAbleSpace(){return buffer_.capacity()-writeAbleIndex_;}

    char* readAbleStart(){return &*readAbleIter();}

    char* writeAbleStart(){return &*writeAbleIter();}

    std::vector<char>::iterator readAbleIter(){return begin()+readAbleIndex_;}
    std::vector<char>::iterator writeAbleIter(){return begin()+writeAbleIndex_;}


    size_t readFd(int fd);


    void append(const char*msg,size_t len){
    
        hasensureWritespace(len);
        std::copy(msg,msg+len,writeAbleIter());
        writeAbleIndex_+=len;
    
    }
    size_t capacity(){return buffer_.capacity();}
    void hasensureWritespace(size_t len){
        if(len > writeAbleSpace()){
            makeSpace(len);
        }
    }
    
    std::string retireveAsString(size_t len){
        
        
        std::string res(readAbleIter(),readAbleIter()+len);
        retireve(len);
        return res;
        
    }
    void retireve(size_t len){
    
        if(len < readAbleSpace()){
            readAbleIndex_+=len;
            
        }else {
       
            retireveAll();
        }
    }
    void retireveAll(){
    
        readAbleIndex_=0;
        writeAbleIndex_=0;

    }
    
    private:

        void makeSpace(size_t len){
       
            if(readAbleSpace() + len <capacity()){
                
                size_t readable=readAbleSpace();
                std::copy(readAbleIter(),writeAbleIter(),begin());
                readAbleIndex_=0;
                writeAbleIndex_=readable; 
            
            }else {
                size_t readAble=readAbleSpace();        
                buffer_.resize(readAble+len);
            }
         /*   if(readAbleIndex_ +writeAbleSpace() < len){
                
                buffer_.resize(writeAbleIndex_+len);
            }else {
                size_t readable=readAbleSpace();
                std::copy(readableIter(),writeableIter(),begin());
                readAbleIndex_=0;
                writeAbleIndex_=readable;

            }
           */ 
        }
        std::vector<char>::iterator begin(){return buffer_.begin();}
        size_t readAbleIndex_;
        size_t writeAbleIndex_;
        std::vector<char> buffer_;


};
/*class Buffer{
    public:
        
        Buffer(size_t bufferSize=kDefaultSize,
             size_t misalign=0  )
            :bufferSize_(bufferSize),
            misalign_(misalign),
            off_(0){
                buffer_=new char[bufferSize];  
            }

        void append(const char*msg,size_t len){
        
            if(len > bufferSize_){
                
                if(shouldRealign(len)){
                    Realign();
                    
                    off_+=len;
                }else {
                    
                    size_t size=bufferSize_+len;
                    reAlloc(size);
                }
            
            } 
        
        
        
        
        }
        
    private:
        void reAlloc(size_t size){
        
        
        
        }
        bool shouldRealign(size_t len){
            
            return bufferSize_-off_ > len;
        }
        void Realign(){
             
            ::memmove(buffer_,buffer_+misalign_+off_,off_);
            misalign_=0;
        }
        size_t bufferSize_;

        size_t misalign_;

        size_t off_; //写的字节数

        char *buffer_;


};


}

*/
}
#endif 
