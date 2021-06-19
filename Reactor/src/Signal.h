
#ifndef SIGNAL_NET_H_
#define SIGNAL_NET_H_

#include "Callback.h"
#include "Channel.h"


#include <arpa/inet.h>
#include <sys/signalfd.h>

#include <memory>
#include <unordered_map>
#include <vector>
namespace net {

    
class SignalPoll{
    public:
        SignalPoll(EventLoop *loop);            
        

        void setSignal(int signo,signalCallback cb);
        void delSignal(int signo,signalCallback cb);

        
    private:
        EventLoop  *loop_;
        sigset_t mask_;
        int signalFd_;
        std::unique_ptr<Channel> signalChannel_;
    
        std::unordered_map<int,signalCallback> signalMap_;
        
        std::vector<int> signalNum_;

        std::vector<struct sigaction> oldSigaction_;

};





}


#endif 

