
#ifndef SIGNAL_NET_H_
#define SIGNAL_NET_H_

#include "Callback.h"
#include "Channel.h"

#include <sys/signal.h>
#include <arpa/inet.h>
#include <sys/types.h>

#include <vector>
#include <unordered_map>
#include <memory>

namespace net {

class EventLoop;

class SignalInfo{

public:
    
    using Functor=std::function<void()>;
    SignalInfo(EventLoop *loop);
    void setSocketpair(){
        if(::socketpair(AF_UNIX,SOCK_STREAM,0,signalPair_)){
            
            perror("socketpair error");
        }
    
    }
    

    int  readSocket(){return signalPair_[0];}


    void setSignalhandler(int signals);
    void setSignal(int signals,Functor func);
    void delSignal(int signals);

    static void sigHandler(int signo);
    void doSignalFunctor();     
private:

    // void sigHandler_(int signo);
    void handlerRead();
    EventLoop *loop_;
    static int signalPair_[2];
    std::unique_ptr<Channel> signalChannel_;
    std::unordered_map<int,signalCallback> signalMap_;
    // std::vector<std::pair<int,signalCallback>> signalMap_;
    std::vector<int> signalNum_;
    
    std::vector<struct sigaction> old;
};

}



#endif 
