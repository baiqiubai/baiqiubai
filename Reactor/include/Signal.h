
#ifndef SIGNAL_NET_H_
#define SIGNAL_NET_H_

#include "Channel.h"
#include "noncopyable.h"
#include "Callback.h"

#include <arpa/inet.h>
#include <sys/signalfd.h>

#include <memory>
#include <unordered_map>
#include <vector>
namespace net {
class EventLoop;
    
class Signal:public base::noncopyable{
    public:
        Signal(EventLoop *loop);            
        

        void setSignal(uint32_t signo,net::signalCallback cb);
        void delSignal(uint32_t signo);

        // int sigIsBlock(uint32_t signo);

            
        void handleRead();
    private:
        EventLoop  *loop_;
        sigset_t mask_;
        int signalFd_;
        std::unique_ptr<Channel> signalChannel_;
    
        std::unordered_map<uint32_t,net::signalCallback> signalMap_;
        
        // std::vector<int> signalNum_;

        // std::vector<struct sigaction> oldSigaction_;

};





}


#endif 

