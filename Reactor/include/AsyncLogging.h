
#ifndef ASYNCLOGGING_BASE_H_
#define ASYNCLOGGING_BASE_H_

#include "LogFile.h"
#include "Thread.h"
#include "Condition.h"
#include "LogStream.h"
#include "CountDownLatch.h"

#include <vector>
namespace base {

class AsyncLogging{

    public:
        AsyncLogging(const std::string basename,
                size_t fileSize=200000,
                size_t fileRows=8888);        
        

        ~AsyncLogging();
        void start();
        void stop();

        void append(const char*msg,size_t len);
    private:

        void threadfunc();
        using Buffer=FixedBuffer<kSmallBufferSize>;
        // using Buffer=FixedBuffer<kLargerBufferSize>;
        using BufferPtr=std::unique_ptr<Buffer>;
        using BufferVec=std::vector<BufferPtr>;
        const std::string baseName_;
        const size_t fileSize_;
        const size_t fileRows_;
        
        std::atomic<bool> start_;
        Thread thread_;
        MutexLock mutex_;
        Condition cond_;
        BufferPtr currentBuffer_;
        BufferPtr nextBuffer_;
    
        CountDownLatch latch_;
        BufferVec buffers_;
};





}

#endif 
