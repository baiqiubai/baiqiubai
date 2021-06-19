
#ifndef DEFAULTPOLLER_NET_H_
#define DEFAULTPOLLER_NET_H_

// #include "Poller.h"
#include "Poll.h"
#include "Epoll.h"
namespace net {

class Poller;
Poller *newDefaultPoller();

}


#endif 
