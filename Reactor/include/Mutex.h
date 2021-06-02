
#ifndef REACTOR_BASE_MUTEX_H
#define REACTOR_BASE_MUTEX_H

#include "noncopyable.h"
#include <assert.h>
#include <pthread.h>


extern void __assert_perror_fail (int errnum,
                                  const char *file,
                                  unsigned int line,
                                  const char *function)
    noexcept __attribute__ ((__noreturn__));

#define MCHECK(ret) ({ __typeof__ (ret) errnum = (ret);         \
                       if (__builtin_expect(errnum != 0, 0))    \
                         __assert_perror_fail (errnum, __FILE__, __LINE__, __func__);})
/*
 #define MCHECK(ret) ({ __typeof__ (ret) errnum = (ret);         \
                       assert(errnum == 0); (void) errnum;})

// #endif
*/
namespace base
{

class MutexLock : noncopyable
{
 public:
  MutexLock()
  {
    MCHECK(pthread_mutex_init(&mutex_, NULL));
  }

  ~MutexLock()
  {
    MCHECK(pthread_mutex_destroy(&mutex_));
  }

  void lock()
  {
    MCHECK(pthread_mutex_lock(&mutex_));
  }

  void unlock() 
  {
    MCHECK(pthread_mutex_unlock(&mutex_));
  }

  pthread_mutex_t* getPthreadMutex() /* non-const */
  {
    return &mutex_;
  }

 private:

  pthread_mutex_t mutex_;
};

class MutexLockGuard : noncopyable
{
 public:
  explicit MutexLockGuard(MutexLock& mutex) 
    : mutex_(mutex)
  {
    mutex_.lock();
  }

  ~MutexLockGuard()
  {
    mutex_.unlock();
  }

 private:

  MutexLock& mutex_;
};
}
#endif
