
#ifndef TCPCLIENT_NET_H_
#define TCPCLIENT_NET_H_

#include "InetAddress.h"
#include "EventLoop.h"
#include "Connector.h"
#include "Channel.h"
#include <memory>
namespace net {

class TcpClient{

    public:
        
        TcpClient(EventLoop *loop,const InetAddress &servaddress);


        void start();
    private:
        
        void OnConnection();
        void handlerRead();
        InetAddress peerAddress_;
        EventLoop *loop_;
        std::unique_ptr<Connector> connector_;
        InetAddress localAddress_;

};









}







#endif
