
#ifndef CONNECTOR_NET_H_
#define CONNECTOR_NET_H_

#include "InetAddress.h"
#include "Socket.h"
#include "Channel.h"

#include <atomic>
#include <memory>
#include <functional>

namespace net {

class EventLoop;
class Connector{

    public:
        
        using NewConnectionCallback=std::function<void()>;
        Connector(EventLoop *loop,const InetAddress &addr);

        
        
        void setNewConnectionCallback(NewConnectionCallback cb){
            
            newConnectionCallback_=std::move(cb);
        }

        void start();
    private:
        void connect();
        enum State {DisConnected,Connecting,Connected};
       

        void setState(State state){state_=state;}
        void startInLoop();
        static size_t kMaxRetryDelay;
        static const size_t kInitretryDelay=500;
        void retry(int sockfd);

        void connecting(int sockfd);
        void handlerWrite();
        int removeAndReset();
        void resetChannel();
        EventLoop *loop_;
                
        InetAddress inetAddress_;
            
        // Socket connectSocket_;
        // int sockfd_;

        std::atomic<bool> connecting_;
        NewConnectionCallback newConnectionCallback_;
        size_t retryDelayms_;
        State state_;    
        std::unique_ptr<Channel> connectChannel_;
};


}

#endif 
