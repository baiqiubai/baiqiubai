
#include "TcpClient.h"

namespace  net {

    TcpClient::TcpClient(EventLoop *loop,
            const InetAddress &servaddress)
    :peerAddress_(servaddress),
    loop_(loop),
    connector_(new Connector(loop_,peerAddress_)){
    
        connector_->setNewConnectionCallback(std::bind(&TcpClient::OnConnection,
                    this));
    

    }
    
   
    void TcpClient::start(){
    
        connector_->start();
        // connector_->connect();
    }

    void TcpClient::OnConnection(){
        
        printf("newConnection\n");
    
    }

    void TcpClient::handlerRead(){
    
    
    
    
    
    
    
    
    
    
    }






}
