
#include "DefaultPoller.h"

namespace net {

Poller *newDefaultPoller(){

    if(::getenv("MY_POLLER_POLL")){
        
        return new Epoll();
    }else {
    
        return new Poll();
    }


}



}

