#include "CurrentThread.h"
#include "TimeStamp.h"
#include <cxxabi.h>
#include <execinfo.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <unistd.h>
namespace base
{
namespace detail{

pid_t gettid(){


    return ::syscall(SYS_gettid);
}

}
namespace CurrentThread
{
__thread int t_cachedTid = 0;
__thread char t_tidString[32];
__thread int t_tidStringLength = 6;
__thread const char* t_threadName = "unknown";
static_assert(std::is_same<int, pid_t>::value, "pid_t should be int");

void cacheTid()
{
  if (t_cachedTid == 0)
  {
    t_cachedTid = detail::gettid();
    t_tidStringLength = snprintf(t_tidString, sizeof t_tidString, "%5d ", t_cachedTid);

  }
}
void sleepUsec(int64_t usec)
{
  struct timespec ts = { 0, 0 };
  ts.tv_sec = static_cast<time_t>(usec / base::TimeStamp::kSecondsToMicroSeconds);
  ts.tv_nsec = static_cast<long>(usec % base::TimeStamp::kSecondsToMicroSeconds * 1000);

  ::nanosleep(&ts,NULL);
}
bool isMainThread(){
    
    return t_cachedTid==::getpid();
}

}
}
