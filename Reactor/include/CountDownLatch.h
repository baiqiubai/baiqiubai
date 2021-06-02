
#ifndef REACTOR_BASE_COUNTDOWNLATCH_H
#define REACTOR_BASE_COUNTDOWNLATCH_H

#include "Condition.h"
#include "Mutex.h"

namespace base
{

class CountDownLatch : noncopyable
{
 public:

  explicit CountDownLatch(int count);

  void wait();

  void countDown();

  int getCount() const;

 private:
  mutable MutexLock mutex_;
  Condition condition_;
  int count_ ;
};
}
#endif 
