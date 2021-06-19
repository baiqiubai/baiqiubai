
#include "Signal.h"

namespace net {

     


    SignalPoll::SignalPoll(EventLoop *loop)
    :loop_(loop)
    {
    
        signalFd_=::signalfd()
    
    }







}
