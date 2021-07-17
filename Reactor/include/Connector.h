#ifndef CONNECTOR_NET_H_
#define CONNECTOR_NET_H_

#include "InetAddress.h"
#include "Socket.h"
#include "Channel.h"
#include "Callback.h"

#include <atomic>
#include <memory>
#include <functional>

namespace net {

class EventLoop;
class Connector{

    public:
        
        Connector(EventLoop *loop,const InetAddress &addr);
        ~Connector();
        
        void setNewConnectionCallback(NewConnectionCallback cb){
            
            newConnectionCallback_=std::move(cb);
        }

        void start();
        void stop();

        void restart();
    private:
        void connect();
        enum State {DisConnected,Connecting,Connected};
       

        void setState(State state){state_=state;}
        void startInLoop();
        void stopInLoop();
        static size_t kMaxRetryDelay;
        static const size_t kInitretryDelay=500;
        void retry(int sockfd);

        void connecting(int sockfd);
        void handleWrite();
        void handleError();//针对POLLHUP 
        int removeAndReset();
        void resetChannel();
        EventLoop *loop_;
                
        InetAddress inetAddress_;
        
        bool connecting_;
        
        size_t retryDelayms_;

        State state_;
        NewConnectionCallback newConnectionCallback_;
        
        std::unique_ptr<Channel> connectChannel_;
};

}
#endif 
