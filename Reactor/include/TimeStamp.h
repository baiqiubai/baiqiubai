#ifndef REACTOR_BASE_TIMESTAMP_H_
#define REACTOR_BASE_TIMESTAMP_H_
#include <iostream>
namespace base {
class TimeStamp{
    public:
        TimeStamp(int64_t microseconds):microSeconds_(microseconds){}
        TimeStamp():microSeconds_(0){}

        int64_t microSeconds()const{return microSeconds_;}

        static TimeStamp now();

        std::string toString()const;
        std::string toFormatString(bool hasMicroSecond=true)const;
    
        static const int kSecondsToMicroSeconds=1000* 1000;
    private:
        int64_t microSeconds_;
};

inline TimeStamp addTime(const TimeStamp &now,double interval)
{

    int64_t microseconds=now.microSeconds();
    return TimeStamp(microseconds+interval *TimeStamp::kSecondsToMicroSeconds);

}
inline bool operator<(const TimeStamp&t1,const TimeStamp&t2){

    return t1.microSeconds()<t2.microSeconds();
}
double timeDifference(const TimeStamp &t1,const TimeStamp &t2); 

}

#endif 
