
#include "ThreadPool.h"

namespace base {

const int kDefaultTaskNum = 10;

ThreadPool::ThreadPool(const threadCallback &cb, std::string name)
    : threadCallback_(cb),
      poolName_(std::move(name)),
      start_(false),
      nextThreadId_(1),
      mutex_(),
      emptyCond_(mutex_),
      fullCond_(mutex_),
      taskNum_(kDefaultTaskNum)
{
}

void ThreadPool::stop()
{
    {
        MutexLockGuard lock(mutex_);

        start_ = false;
        emptyCond_.broadcast();

        fullCond_.broadcast();
    }
    for (auto &thread : threads_) thread->stop();
}

void ThreadPool::start(int threadNum)
{
    start_ = true;
    threads_.reserve(threadNum);
    for (int i = 0; i < threadNum; ++i) {
        char buf[20] = {0};
        snprintf(buf, sizeof(buf), "%d", i + 1);
        std::string threadName = poolName_ + ":";
        threadName += buf;
        threads_.emplace_back(
            new Thread(std::bind(&ThreadPool::runThread, this), threadName));
        threads_[i]->start();
    }
    if(threadNum==0&&threadCallback_)
        threadCallback_();
}
bool ThreadPool::full()
{
    return taskNum_ > 0 && static_cast<int>(taskQueue_.size()) == taskNum_;
}
void ThreadPool::dispatchTask(threadCallback cb)
{
    if (threads_.empty()) {
        cb();
    }
    else {
        MutexLockGuard lock(mutex_);
        while (start_ && full() ) {
            fullCond_.wait();
        }
        if (!start_) return;
        taskQueue_.emplace_back(std::move(cb));
        emptyCond_.signal();
    }
}
void ThreadPool::runThread()
{
    // MutexLockGuard lock(mutex_);

    if(threadCallback_){
        threadCallback_();
    }
    while (start_) {
        threadCallback cb(take());
        if (cb) cb();
    }
    /*    while (start_) {
            while (taskQueue_.empty()&&start_) {
                emptyCond_.wait();
            }
            if(!start_)return ;
            threadCallback cb = taskQueue_.front();
            taskQueue_.pop_front();
            cb();
            fullCond_.signal();
        }*/
}
ThreadPool::threadCallback ThreadPool::take()
{
    MutexLockGuard lock(mutex_);
    threadCallback cb = NULL;
    while (start_ && taskQueue_.empty()) emptyCond_.wait();

    if (!taskQueue_.empty()) {
        cb = taskQueue_.front();
        taskQueue_.pop_front();
        fullCond_.signal();
    }
    return cb;
}
}  
