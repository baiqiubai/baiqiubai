
#ifndef TCPCLIENT_NET_H_
#define TCPCLIENT_NET_H_

#include "InetAddress.h"
#include "EventLoop.h"
#include "Connector.h"
#include "Channel.h"
#include "Callback.h"
#include "Buffer.h"
#include "noncopyable.h"


#include <memory>
namespace net {

class TcpClient:public base::noncopyable{

    public:
        
        TcpClient(EventLoop *loop,
                const InetAddress &servaddress,
                const std::string name="");
        
        ~TcpClient();
        void setConnectionCallback(ConnectionCallback cb){
        
            connectionCallback_=std::move(cb);
        }
    
        void setMessageCallback(MessageCallback cb){
            messageCallback_=std::move(cb);
        }
        void setWriteCompleteCallback(WriteCompleteCallback cb){
        
            writeCompleteCallback_=std::move(cb);
        }
        void start();
        void stop();
        
        bool retry()const{return  retry_;}
        void enableRetry(){retry_=true;}
        void removeConnection(const TcpConnectionPtr&);
    private:
        
        void OnConnection(int sockfd);
        InetAddress peerAddress_;
        EventLoop *loop_;
        std::unique_ptr<Connector> connector_;
        InetAddress localAddress_;
        
        bool retry_;
        std::string name_;
        

        TcpConnectionPtr  connPtr_;
        ConnectionCallback connectionCallback_;
        MessageCallback messageCallback_;
        
        WriteCompleteCallback writeCompleteCallback_;
};









}







#endif
