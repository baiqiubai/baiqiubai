#ifndef REACTOR_BASE_NONCOPYABLE_H
#define REACTOR_BASE_NONCOPYABLE_H

namespace base
{

class noncopyable
{
 public:
  noncopyable(const noncopyable&) = delete;
  void operator=(const noncopyable&) = delete;

 protected:
  noncopyable() = default;
  ~noncopyable() = default;
};

}  

#endif
