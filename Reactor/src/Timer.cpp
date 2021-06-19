
#include "Timer.h"

namespace net  {

std::atomic_int Timer::Num_(1);
void Timer::resetTimer(base::TimeStamp now){


    if(repeat_){
    
       timer_=addTime(now,interval_); 
    }else {
        
        timer_=base::TimeStamp();
    
    }



}








}
