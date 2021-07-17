
#include "Connector.h"
#include "EventLoop.h"
#include "LogStream.h"
#include "Logger.h"
#include "SocketOps.h"

#include <algorithm>

using namespace base;
namespace net {

size_t Connector::kMaxRetryDelay = 500 * 100;

Connector::Connector(EventLoop *loop, const InetAddress &addr)
    : loop_(loop),
      inetAddress_(addr),
      connecting_(false),
      retryDelayms_(kInitretryDelay),
      state_(DisConnected)
{
}
Connector::~Connector(){
    
    LOG_DEBUG<<"Connector::~Connector ";
}
void Connector::connect()
{  //每次连接重新创建socket 否则出现ECONNABORTED

    int sockfd = sockets::createNonBlockingSockfd();
    int ret = sockets::connect(sockfd, &inetAddress_);
    int saveErrno= (ret==0)?0:errno;
    if (ret < 0) {
        switch (saveErrno) {
            case 0:
            case EINTR:
            case EISCONN:
            case EINPROGRESS:
                connecting(sockfd);
                break;
            case ECONNREFUSED:
            case EAGAIN:
                retry(sockfd);
                break;
            default:
                break;
        }
    }
}
void Connector::start()
{
    connecting_ = true;
    loop_->runInLoop(std::bind(&Connector::startInLoop, this));
}
void Connector::stop()
{
    connecting_ = false;
    loop_->runInLoop(std::bind(&Connector::stopInLoop, this));
}
void Connector::stopInLoop()
{
    LOG_DEBUG << "Connector::stopInLoop State " << state_;
    if (connecting_) {
    }
}
void Connector::restart()
{
    setState(DisConnected);
    retryDelayms_ = kInitretryDelay;
    connecting_ = true;
    startInLoop();
}
void Connector::startInLoop()
{
    loop_->assertInLoop();

    if (state_ == DisConnected) {
        connect();
    }
}

int Connector::removeAndReset()
{
    connectChannel_->disableAll();
    connectChannel_->remove();
    int sockfd = connectChannel_->fd();
    loop_->queueInLoop(std::bind(&Connector::resetChannel, this));
    return sockfd;
}
void Connector::resetChannel() { connectChannel_.reset(); }
void Connector::handleWrite()
{
    if (state_ == Connecting) {
        int sockfd = removeAndReset();
        int err = sockets::getSocketError(sockfd);
        if (err) {
            retry(sockfd);
        }
        else if (sockets::isSelfConnect(sockfd)) {
            retry(sockfd);
        }
        else {
            setState(Connected);
            if (newConnectionCallback_) newConnectionCallback_(sockfd);
        }
    }
}
void Connector::handleError()
{
    LOG_DEBUG<<"Connector::handleError ";
    if (state_ == DisConnected) {
        int sockfd = removeAndReset();
        retry(sockfd);
    }
}
void Connector::connecting(int sockfd)
{
    LOG_DEBUG << "connecting sockfd: " << sockfd;

    if (connecting_) {
        setState(Connecting);
        connectChannel_.reset(new Channel(loop_, sockfd));
        connectChannel_->setWriteCallback(
            std::bind(&Connector::handleWrite, this));
        connectChannel_->setErrorCallback(
            std::bind(&Connector::handleError, this));
        connectChannel_->enableWriting();
    }else {
        
    }


}
void Connector::retry(int sockfd)
{
    LOG_DEBUG << "Connector::retry sockfd: " << sockfd << "connecting_ " << connecting_;
    if (connecting_) {
        setState(DisConnected);
        sockets::close(sockfd);
        loop_->runAfter(std::bind(&Connector::startInLoop, this),
                        retryDelayms_ / 1000.0);
        retryDelayms_ = std::min(2 * retryDelayms_, kMaxRetryDelay);
    }
    else {
        LOG_DEBUG << "Connector::stop Connector";
    }
}

}  // namespace net

