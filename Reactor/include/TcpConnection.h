
#ifndef TCPCONNECTION_NET_H_
#define TCPCONNECTION_NET_H_

#include "Buffer.h"
#include "Callback.h"
#include "Channel.h"
#include "InetAddress.h"
#include "Socket.h"
#include <memory>

namespace net {

class EventLoop;
class TcpConnection : public std::enable_shared_from_this<TcpConnection> {
public:
    TcpConnection(EventLoop *loop, const InetAddress &localAddress,
                  const InetAddress &peerAddress, int sockfd, std::string name);

    void setMessageCallback( MessageCallback cb)
    {
        messageCallback_ = std::move(cb);
    }
    void setConnectionCallback( ConnectionCallback cb)
    {
        connectionCallback_=std::move(cb);
    }

    void setCloseConnectionCallback(CloseConnectionCallback cb)
    {
        closeCallback_=std::move(cb);
    }
    void setWriteCompleteCallback(WriteCompleteCallback cb){
    
        writeCompleteCallback_=std::move(cb);
    }
    void setHighWaterMarkCallback(HighWaterMarkCallback cb){
    
        highWaterMarkCallback_=std::move(cb); 
    }
    std::string name() const { return connName_; }
    void removeConnectionInPoller();

    ~TcpConnection();

    InetAddress &localAddress(){return localAddress_;}
    InetAddress &peerAddress(){return peerAddress_;}

    // void send(const std::string &message);
    void send(const char*message,size_t datalen);

    enum State { Connecting, Connected, DisConnecting, DisConnected };
    void setState(State s) { state_ = s; }
    State state() { return state_; }

    void shutdown();

    void connectEstablished();

    EventLoop *getLoop(){return loop_;}


    void socketOutbuf(); //给outputbuffer使用
private:
    // void sendInLoop(const std::string& message);
    void sendInLoop(const char *message,size_t datalen);


    void shutdownInLoop();
    void handleWrite();
    void handleClose();
    void handleError();
    void handleRead();
    EventLoop *loop_;
    InetAddress localAddress_;
    InetAddress peerAddress_;
    Socket tcpSocket_;

    std::unique_ptr<Channel> tcpConnectionChannel_;
    std::string connName_;
    State state_;
    
    size_t highWaterMark_;
    CloseConnectionCallback closeCallback_;
    ConnectionCallback connectionCallback_;
    MessageCallback messageCallback_;
    WriteCompleteCallback writeCompleteCallback_;
    HighWaterMarkCallback highWaterMarkCallback_;
    Buffer inputBuffer_;
    Buffer outputBuffer_;

    bool tcpWriting_; 
};

}  // namespace net

#endif
