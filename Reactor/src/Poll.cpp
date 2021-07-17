
#include "Poll.h"
#include "Channel.h"
#include "Logger.h"
#include "LogStream.h"


#include <assert.h>
#include <unistd.h>
using namespace base;
namespace net {

Poll::Poll()
:Poller(){


}
Poll::~Poll(){


}
void Poll::update(Channel *channel)
{
    
    if (channel->index() < 0) {  // new add
        
        int fd = channel->fd();

        LOG_DEBUG<<"Poll::update fd: "<<fd<<" Operation:POLL_ADD ";

        struct pollfd pfd;
        

        
        pfd.fd = fd;
        pfd.events = channel->events();
        pfd.revents = 0;

        pollfds_.emplace_back(pfd);
        
        int index = static_cast<int>(pollfds_.size()) - 1;
    
        channel->setIndex(index);

        channelMap_.insert({fd, channel});
    }
    else {

        int index = channel->index();
        struct pollfd &pfd = pollfds_[index];
        
        pfd.fd = channel->fd();
        
        LOG_DEBUG<<"Poll::update fd: "<<pfd.fd<<" Operation:POLL_DEL ";
        pfd.events =channel->events();
        pfd.revents = 0;
                
        
        if(channel->isNoneEvent()){
            pfd.fd=-1; 
        }
        
    }
}
void Poll::remove(Channel *channel)
{
    LOG_DEBUG<<"Poll::remove fd: "<<channel->fd();
    if (pollfds_.size() > 0) {
        assert(channel->fd() >= 0);
        
    
        int fd = channel->fd();
        int index = channel->index();
        if (channel->index() == static_cast<int>(pollfds_.size()) - 1) {
            
            pollfds_.pop_back();
        }
        else {
            
            
            int endfd = pollfds_.back().fd;
            std::swap(pollfds_[index], pollfds_.back());
            channelMap_[endfd]->setIndex(index);  //设置为删除掉channel的index
            pollfds_.pop_back();
        }
        size_t n = channelMap_.erase(fd);
        assert(n == 1);
    }
}
void Poll::fillActiveChannel(std::vector<Channel*> *activeChannel,int nready)
{
    for (auto it = pollfds_.begin(); it != pollfds_.end(); ++it) {
        if (it->revents > 0) {
            int fd = it->fd;

            int revents = it->revents;
        
            auto pos = channelMap_.find(fd);
            assert(pos != channelMap_.end());

            assert(pos->first == fd);

            pos->second->setRevents(revents);

            activeChannel->emplace_back(pos->second);
        }
    }
}
void Poll::poll(std::vector<Channel*> * activeChannel,
int timeout)
{
    // again:

    int nready = ::poll(&*pollfds_.begin(), pollfds_.size(), timeout);

    if (nready >= 0) {
        fillActiveChannel(activeChannel,nready);
    }
    else if (nready < 0) {
        if (errno == EINTR) {
            // goto again;
        }
        else {
            LOG_ERROR<<"poll error";
        }
    }
}

}  // namespace net
