
#include "LogStream.h"
#include <algorithm>
namespace base {

const char* digits = "0123456789";

template <class T>
size_t tranToString(char* buf, T val)
{
    char* p = buf;
    T t = 0;
    if (val < 0)
        t = -val;
    else
        t = val;
    do {
        int index = static_cast<int>(t % 10);
        t /= 10;
        *p++ = digits[index];

    } while (t);

    if (val < 0) {
        *p++ = '-';
    }
    *p = '\0';
    std::reverse(buf, p);
    return p - buf;
}
template <class T>
void LogStream::Interger(T val)
{
    size_t n = tranToString(buffer_.current(), val);
    buffer_.add(n);
}
LogStream& LogStream::operator<<(long long val)
{
    Interger(val);
    return *this;
}

LogStream& LogStream::operator<<(long int val)
{
    Interger(val);
    return *this;
}
LogStream& LogStream::operator<<(int val)
{
    Interger(val);
    return *this;
}
LogStream& LogStream::operator<<(unsigned int val)
{
    Interger(val);
    return *this;
}
LogStream& LogStream::operator<<(unsigned long long val)
{
    Interger(val);
    return *this;
}
LogStream& LogStream::operator<<(double val)
{
    size_t len = snprintf(buffer_.current(), 32, "%.12f", val);
    buffer_.add(len);
    return *this;
}

}  // namespace base
