
#include "Signal.h"
#include "EventLoop.h"

#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>
#include <assert.h>
namespace net {

     


    Signal::Signal(EventLoop *loop)
    :loop_(loop)
    {
    
        ::sigemptyset(&mask_);
        signalFd_=::signalfd(-1,&mask_,SFD_NONBLOCK|SFD_CLOEXEC);
        signalChannel_.reset(new Channel(loop_,signalFd_));
        
        signalChannel_->setReadCallback(std::bind(&Signal::handleRead,this));
        signalChannel_->enableReading();
        signalMap_.reserve(32);
    }
    
    void Signal::handleRead(){
        
        struct signalfd_siginfo sigInfo;

        size_t n=::read(signalFd_,&sigInfo,sizeof(sigInfo));

        assert(n==sizeof(sigInfo));

        signalMap_[sigInfo.ssi_signo]();
    }

    void Signal::setSignal(uint32_t signo,signalCallback cb){
        
        ::sigaddset(&mask_,signo);
        ::sigprocmask(SIG_BLOCK,&mask_,NULL);
        signalMap_[signo]=std::move(cb);
        ::signalfd(signalFd_,&mask_,SFD_NONBLOCK|SFD_CLOEXEC);

    }
    void Signal::delSignal(uint32_t signo){
        
        
        auto pos=signalMap_.find(signo);
        if(pos!=signalMap_.end()){
            ::sigdelset(&mask_,signo);
            ::sigprocmask(SIG_SETMASK,&mask_,NULL);
            signalMap_.erase(pos);
        }

    } 




}
