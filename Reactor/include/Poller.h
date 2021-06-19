
#ifndef POLLER_NET_H_
#define POLLER_NET_H_

#include <vector>
#include <unordered_map>
namespace net {

class Channel;
class Poller{


    public:
        
        Poller()=default;

        virtual void update(Channel *channel)=0;

        virtual void remove(Channel *channel)=0;

        // virtual void poll(std::vector<Channel*> *signalChannel,
                // std::vector<Channel*> *timerChannel,
                // std::vector<Channel*> *activeChannel,
                // int timeout)=0;
    
        virtual void poll(std::vector<Channel*> *activeChannel,int timeout)=0;
        virtual ~Poller(){}
        virtual  void fillActiveChannel(std::vector<Channel*> *activeChannel,int nready)=0;
    protected:
        using ChannelMap=std::unordered_map<int,Channel*> ;

        ChannelMap channelMap_;
    

};



}


#endif 
