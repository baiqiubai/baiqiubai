
#include "TimeStamp.h"

#include <sys/time.h>
#include <inttypes.h>
#include <cstring>
namespace base{

   TimeStamp TimeStamp::now(){
    
    
       struct timeval val;
       memset(&val,'\0',sizeof(val));
       ::gettimeofday(&val,NULL);

       return TimeStamp(val.tv_sec*kSecondsToMicroSeconds+val.tv_usec);
    }

    std::string TimeStamp::toString()const{
    
        char buf[32]={0};
        int64_t seconds=microSeconds_ / kSecondsToMicroSeconds;

        int64_t microseconds=microSeconds_ %kSecondsToMicroSeconds;
        snprintf(buf,sizeof(buf),"%" PRId64 ".%.06" PRId64 "",seconds,microseconds);
        return buf;
        
    
    }

    std::string TimeStamp::toFormatString(bool hasMicroSeconds)const{
    
            struct tm tm_time;

            time_t time=static_cast<time_t>(microSeconds_ / kSecondsToMicroSeconds);
            ::gmtime_r(&time,&tm_time);
            char buf[32]={0};
            if(!hasMicroSeconds){
                
                snprintf(buf,sizeof(buf),"%04d%02d%02d-%02d:%02d:%02d",
                        tm_time.tm_year+1900,tm_time.tm_mon+1,tm_time.tm_mday,
                        tm_time.tm_hour,tm_time.tm_min,tm_time.tm_sec);
                
            }else {
                
                int microseconds=static_cast<int>(microSeconds_ %kSecondsToMicroSeconds);
                snprintf(buf,sizeof(buf),"%04d%02d%02d-%02d:%02d:%02d.%.06d",
                        tm_time.tm_year+1900,tm_time.tm_mon+1,tm_time.tm_mday,
                        tm_time.tm_hour,tm_time.tm_min,tm_time.tm_sec,microseconds);
                

            }
            return buf;
            
    
    }
double timeDifference(const TimeStamp&t1,const TimeStamp&t2){
    
    double dif=0;

  /*  if(t1 <  t2 ){
    
        dif=static_cast<double>(t2.microSeconds()-t1.microSeconds())/TimeStamp::kSecondsToMicroSeconds;
    
    }else 
        dif=static_cast<double>(t1.microSeconds()-t2.microSeconds())/TimeStamp::kSecondsToMicroSeconds;
*/
    dif=static_cast<double>(t1.microSeconds()-t2.microSeconds())/TimeStamp::kSecondsToMicroSeconds;
    return dif;

}
}
