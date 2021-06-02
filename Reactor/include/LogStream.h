
#ifndef LOGSTREAM_BASE_H_
#define LOGSTREAM_BASE_H_
#include <iostream>
#include <cstring>
namespace base {

const int kSmallBufferSize=4000;
const int kLargerBufferSize=4000*1000;
template<int SIZE>
class FixedBuffer{
    public:
        FixedBuffer():cur_(data_),heap_(data_),size_(SIZE+1){
                    //make_heap
            bZero();
        }
        
        void append(const char*msg,int len){
            if(valid()>=len){
            
                memcpy(cur_,msg,len);
                cur_+=len;
                heap_[size_-1]='\0';
            }else {
                makeHeap(len);
                memcpy(cur_,msg,len);
                cur_+=len;
                heap_[size_-1]='\0';
            }
            
        }
        int valid()const {return end()-cur_;}
        
        void add(size_t len){cur_+=len;}
        char*current(){return cur_;}
        const char*end()const{return heap_+size_-1; }
        

        char *data(){return heap_;}
        
        size_t dataSize()const{return cur_-heap_;}
        size_t size()const{return size_;}
        void bZero(){memset(heap_,'\0',size_);
            cur_=heap_;
        }
        std::string toString()const{return std::string(heap_,cur_);}
    private:
        void makeHeap(int len){
            char *old=heap_;
            int startIndex=cur_-heap_;
     
            while(valid()<len){
                size_*=2; 
            }
            heap_=new char[size_];
            memcpy(heap_,old,startIndex);
            cur_=heap_+startIndex;
             
        }
        char *cur_;
        char *heap_;
        size_t size_;
        char data_[SIZE+1];
};
using Buffer=FixedBuffer<kSmallBufferSize>;
class LogStream{
    public:

        LogStream& operator<<(int );
        LogStream& operator<<(long int);
        LogStream& operator<<(long long);
        LogStream& operator<<(double );
        LogStream& operator<<(unsigned int );
        LogStream& operator<<(unsigned long long);
        LogStream& operator<<(float val){
            
            *this<<static_cast<double>(val);
            return *this;
        }
        LogStream &operator<<(const char* val){
            if(val){
            
                buffer_.append(val,strlen(val));
            }else {
                buffer_.append("(NULL)",6);
            }
            return *this;
        }
        LogStream& operator<<(char c){
            
            buffer_.append(&c,1);
            return *this;
        }
        LogStream &operator<<(const std::string val){
                
            buffer_.append(val.c_str(),val.size());    
            return *this;
        }
        
        LogStream& operator<<(const void* val){
            *this<<reinterpret_cast<const char*>(val);            
            return *this;
        }
        void append(const char*msg,size_t len){buffer_.append(msg,len);}
        const Buffer& getBuffer()const{return buffer_;}
    
        Buffer &getBuffer(){return buffer_;}
        template<class T>
        void Interger(T val);
    private:

        Buffer buffer_;
};

}

#endif 
