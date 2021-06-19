
#include "AsyncLogging.h"

namespace base {

AsyncLogging::AsyncLogging(const std::string basename,
        size_t fileSize,
        size_t fileRows)
    :baseName_(basename),
    fileSize_(fileSize),
    fileRows_(fileRows),
    start_(false),
    thread_(std::bind(&AsyncLogging::threadfunc,this)),
    mutex_(),
    cond_(mutex_),
    currentBuffer_(new Buffer()),
    nextBuffer_(new Buffer()),
    latch_(1)
    {
    

    
    }

    void AsyncLogging::start(){
    
        start_=true;
        thread_.start();
        latch_.wait();
        // latch_.countDown();
    }
    void AsyncLogging::stop(){
        
        if(start_){
            start_=false;
            cond_.signal();  
            thread_.stop();
        }


    }
    AsyncLogging::~AsyncLogging(){
        if(start_){
            stop();
        }

    }

    void AsyncLogging::append(const char*msg,size_t len){
            
        MutexLockGuard lock(mutex_);
        if(static_cast<size_t>(currentBuffer_->valid()) > len)
            currentBuffer_->append(msg,len);
        else {
            buffers_.emplace_back(std::move(currentBuffer_));
            if(nextBuffer_){
                
                currentBuffer_=std::move(nextBuffer_);
            }else {
                currentBuffer_.reset(new Buffer());
            }
            currentBuffer_->append(msg,len);    
            cond_.signal();
        } 

        
    
    }
    void AsyncLogging::threadfunc(){
            
            
            latch_.countDown();
            BufferPtr buffer1(new Buffer());
            BufferPtr buffer2(new Buffer());
            
            BufferVec bufferToWrite;
            
            LogFile file_(baseName_,fileSize_,fileRows_);
            // latch_.wait();
            while(start_){
            
            
                {
                
                    MutexLockGuard lock(mutex_);
                    // while(buffers_.empty()){
                        // cond_.waitForSeconds(1);
                        //
                        // cond_.wait();
                    // }
                    //
                    if(buffers_.empty()){
                        
                        cond_.waitForSeconds(1);
                    }
                    buffers_.emplace_back(std::move(currentBuffer_));
                    currentBuffer_=std::move(buffer1);
                    bufferToWrite.swap(buffers_);
                    if(!nextBuffer_){
                        nextBuffer_=std::move(buffer2);
                    }
                }

            
                assert(!bufferToWrite.empty());
                for(auto &buffer:bufferToWrite)
                    file_.append(buffer->data(),buffer->dataSize());
                
                
                 if(!buffer1){
                    
                     assert(!bufferToWrite.empty());   
                     buffer1=std::move(bufferToWrite.back());
                     bufferToWrite.pop_back();
                     buffer1->bZero();
                 }   
                 if(!buffer2){
                     assert(!bufferToWrite.empty());   
                     buffer2=std::move(bufferToWrite.back());
                     bufferToWrite.pop_back();
                     buffer2->bZero();
                 }
                    
                bufferToWrite.clear();

                file_.flush();
            
            }
    }






}
