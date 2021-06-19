
#ifndef ACCEPTOR_NET_H_
#define ACCEPTOR_NET_H_

#include "Socket.h"
#include "InetAddress.h"
#include "Channel.h"
#include "Callback.h"

#include <memory>
#include <atomic>
namespace net {

// class Channel;
class EventLoop;
class Acceptor{

    public:
        Acceptor(EventLoop *loop,const InetAddress &addr);
        
        void setNewConnectionCallback(NewConnectionCallback cb){newConnectionCallback_=std::move(cb);}
        
        InetAddress inetAddress()const {return inetAddress_;}
        void listen();

        void accept();
    private:
        
        void bind(InetAddress *address);
        void handlerRead();
        EventLoop *loop_;
        std::atomic<bool> listening_;
        InetAddress inetAddress_;
        Socket socket_;
        std::unique_ptr<Channel> socketChannel_;

        NewConnectionCallback newConnectionCallback_;
};





}



#endif 
