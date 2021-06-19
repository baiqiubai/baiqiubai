
#ifndef POLL_NET_H_
#define POLL_NET_H_

#include "signal.h"
#include "Poller.h"

#include <poll.h>

#include <vector>
#include <memory>
#include <unordered_map>
namespace net {

class Channel;
class Poll:public Poller{



    public:
        using Functor=std::function<void()>;
        
        Poll();
        ~Poll();
        virtual void update(Channel *channel) override;
        virtual void remove(Channel *channel) override;

        // void poll(std::vector<Channel*> *signalChannel,
                // std::vector<Channel*> *timerChannel,
                // std::vector<Channel*> *activeChannel_,
                // int timeout) override;

        virtual void poll(std::vector<Channel*> *activeChannel,int timeout);
        // void setSignalSocketPair(int fd){signalSocketPair_=fd;}
        // void setTimerFd(int fd){timerfd_=fd;}
        // int listenSignalSocketPair()const{return signalSocketPair_;}
        virtual void fillActiveChannel(std::vector<Channel*> *activeChannel,int nready) override;
    private:
    

        // void fillActiveChannel(
                // std::vector<Channel*> *signalChannel,
                // std::vector<Channel*> *timerChannel,
                // std::vector<Channel*> *activeChannel,
                // int nready);
        // using ChannelMap=std::unordered_map<int,Channel*>;

        std::vector<struct pollfd> pollfds_;
        // ChannelMap channelMap_;

        // int signalSocketPair_; //用来监听信号 管道的一端 将信号事件加到signalChannel
        // int timerfd_; //用来判断定时器上的事件 将定时器事件加到 timerChannel
};


}


#endif 
