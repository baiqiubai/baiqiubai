

#ifndef EPOLL_NET_H_
#define EPOLL_NET_H_

#include "Poller.h"

#include <sys/epoll.h>
namespace net {


class Epoll:public Poller{

    public:
        Epoll();
        ~Epoll();
        virtual void update(Channel *channel) override;
    
        virtual void remove(Channel *channel ) override;

        virtual void poll(std::vector<Channel*> *activeChannel,int timeout);
        // virtual void poll(std::vector<Channel*>  *signalChannel,
                // std::vector<Channel*> *timerChannel,
                // std::vector<Channel*> *activeChannel,
                // int timeout) override;
        virtual void fillActiveChannel(std::vector<Channel*> *activeChannel,int nready) override;
    private:
    
        void update(Channel *channel,int operation);
        static const int kInitEventSize=20;
        const int epollfd_; 
        std::vector<struct epoll_event> events_;


};









}


#endif 


