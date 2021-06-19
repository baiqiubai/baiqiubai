
#include "signal.h"
#include "EventLoop.h"
#include "Channel.h"

#include <assert.h>
#include <cstring>
namespace net {
    int SignalInfo::signalPair_[2]={0};
    SignalInfo::SignalInfo(EventLoop *loop)
    :loop_(loop)
    {
    
        setSocketpair();
        
        int &fd=signalPair_[0];

        signalChannel_.reset(new Channel(loop_,fd));
       signalChannel_->setReadCallback(std::bind(&SignalInfo::handlerRead,
                    this));

        // signalChannel_->setReadCallback([&]{this->handlerRead();});
        signalChannel_->enableReading();
        
        signalNum_.reserve(32);
    
        // signalMap_.reserve(32);

        old.reserve(32);
    }
    
    void SignalInfo::handlerRead(){
       
        char signals[1024];
        while(1){
            again: 
            size_t n=::recv(signalPair_[0],signals,sizeof(signals),0);
            if(static_cast<int>(n)==-1){
                if(errno==EINTR)goto again;
                else perror("recv signal error");
                break;
            }else if(n==0){
                
                break;
            }else {
                for(size_t i=0;i<n;++i){
                    if(signals[i])signalNum_[signals[i]]++;
                }
                doSignalFunctor();     
                break;
            }
        }
    
    }


    void SignalInfo::setSignal(int signals,Functor func){
    
        
    
        setSignalhandler(signals);
        signalMap_[signals]=func;

    }
    void SignalInfo::delSignal(int signals){
        
            struct sigaction sh;
            size_t n=signalMap_.erase(signals);
            assert(n==1);
            sh=old[signals];

            if(::sigaction(signals,&sh,NULL)==-1){
                perror("sigaction error");
                
            }
                    
    }
    void SignalInfo::doSignalFunctor(){

    
        size_t size=signalNum_.size();
        for(size_t i=0;i<32;++i){
            if(signalNum_[i]){
                if(signalMap_[i])signalMap_[i]();
                signalNum_[i]--;
            }
        }
    
    }
    void SignalInfo::sigHandler(int signo){
    
        int saveerrno=errno;
        ssize_t n=::send(signalPair_[1],reinterpret_cast<char*>(&signo),sizeof(signo),0);
        if(n==-1){
            perror("send error");
        }
        errno=saveerrno;
    }
    void SignalInfo::setSignalhandler(int signals){
    
        struct sigaction sa; 
        
        memset(&sa,'0',sizeof(sa));
        
        sa.sa_handler=sigHandler;
        sa.sa_flags|=SA_RESTART;
        sigfillset(&sa.sa_mask);

        if(::sigaction(signals,&sa,&old[signals])){
            perror("sigaction error");
        }

    }




}
