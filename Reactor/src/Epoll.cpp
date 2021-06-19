
#include "Epoll.h"
#include "Channel.h"

#include <unistd.h>
#include <assert.h>
namespace net {
namespace {
    
const int kNew=-1;
const int kAdded=0;
const int kDeleted=2;

}

    Epoll::Epoll()
    :Poller(),
    epollfd_(::epoll_create1(EPOLL_CLOEXEC)),
    events_(kInitEventSize){
    
        if(epollfd_ < 0){
            
            perror("epoll_create error");
        }
    
    }
    Epoll::~Epoll(){
        
        if(::close(epollfd_)){
        
        
        }
    }
    
    
    
    

    void Epoll::remove(Channel *channel){
    
        int index=channel->index();
    
        int fd=channel->fd();
        assert(channelMap_.find(fd)!=channelMap_.end());

        assert(index==kDeleted||index==kAdded);

        size_t n=channelMap_.erase(fd);
        assert(n==1);

        
    
    }

    void Epoll::update(Channel *channel){
        
      
        int index=channel->index();
        int fd=channel->fd();
        if(index==kNew || index==kDeleted){  //采用epoll_add
                
               if(index==kNew){
                    
                   assert(channelMap_.find(fd)==channelMap_.end());
                   channelMap_[fd]=channel;
               }else {
                
                    assert(channelMap_.find(fd)!=channelMap_.end());
               } 

              channel->setIndex(kAdded);
              update(channel,EPOLL_CTL_ADD);

        
        }else {//采用Epoll_mod
             
            assert(index==kAdded); 
            if(channel->isNoneEvent()){
                update(channel,EPOLL_CTL_DEL);
                channel->setIndex(kDeleted);
            }else {
            
                update(channel,EPOLL_CTL_MOD);
            }
        }

        
       
    }

    void Epoll::update(Channel *channel,int operation){
    
            
        int fd=channel->fd();
        assert(channelMap_.find(fd)!=channelMap_.end());
    
        struct epoll_event ep;
        ep.data.ptr=channel;
        if(::epoll_ctl(epollfd_,operation,fd,&ep)){
         
        }
            
    }
    
    void Epoll::fillActiveChannel(std::vector<Channel*> *activeChannel,int nready){
    
        for(int i=0;i<nready;++i){
        
            Channel *channel=static_cast<Channel*>(events_[i].data.ptr);
            int fd=channel->fd();
            auto pos=channelMap_.find(fd);
            assert(pos!=channelMap_.end());


            channel->setRevents(events_[i].events);
            activeChannel->emplace_back(channel); 
        }


    }
    void Epoll::poll(std::vector<Channel*> *activeChannel,int timeout){
    
        int numsEvent=::epoll_wait(epollfd_,&*events_.begin(),events_.size(),timeout);
        if(numsEvent >0 ){
        
            fillActiveChannel(activeChannel,numsEvent);
            if(static_cast<size_t>(numsEvent)==events_.size()){
                events_.resize(2*events_.size());
            
            }
        }
    
    }


}
