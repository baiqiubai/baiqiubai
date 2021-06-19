
#ifndef TCPSERVER_NET_H_
#define TCPSERVER_NET_H_

#include "InetAddress.h"
#include "TcpConnection.h"
#include "EventLoopThreadPool.h"
#include "Acceptor.h"
#include "Callback.h"

#include <unordered_map>
namespace net {

class TcpServer{

    public:
        TcpServer(EventLoop *loop,const InetAddress &address,
                std::string name="");


        void start();
        
        void setMessageCallback(MessageCallback cb){
                
            messageCallback_=std::move(cb);
        }
        void setConnectionCallback(ConnectionCallback cb){
        
            connectionCallback_=std::move(cb);
        }
        void setWriteCompleteCallback(WriteCompleteCallback cb){
        
            writeCompleteCallback_=std::move(cb);
        }

        void setHighWaterMarkCallback(HighWaterMarkCallback cb){
            
            highWaterMarkCallback_=std::move(cb);
        }
        
        EventLoop *getLoop(){return loop_;}

        void setThreadNum(int threadNum);
    private:
        
        void removeConnection(const TcpConnectionPtr&conn);
        void closeConnection(const TcpConnectionPtr&conn);
        
        
        using ConnectionMap=std::unordered_map<std::string,
              std::shared_ptr<TcpConnection>> ;
        void OnConnection(int sockfd);
        EventLoop *loop_;
        
        
        std::shared_ptr<Acceptor> acceptor_;
        std::string name_;

        std::unique_ptr<EventLoopThreadPool> threadPool_;
        ConnectionMap connections_;
        
        WriteCompleteCallback writeCompleteCallback_;
        HighWaterMarkCallback highWaterMarkCallback_;
        ConnectionCallback connectionCallback_;
        MessageCallback messageCallback_;
};







}


#endif 
