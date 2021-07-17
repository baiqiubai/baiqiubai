
#include "Channel.h"
#include "EventLoop.h"
#include "Logger.h"
#include "LogStream.h"


#include <assert.h>

using namespace base;
namespace net {
namespace detail{

    std::string getReventsName(int revents){
        
        std::string res("[");
        if(revents & POLLIN)
            res+="POLLIN ";
        if(revents & POLLOUT)
            res+="POLLOUT ";
        if(revents & POLLERR)
            res+="POLLERR ";
        if(revents & POLLHUP)
            res+="POLLHUP ";
        res+="]";
        return res;

    }


}
Channel::Channel(EventLoop *loop,int fd)
:loop_(loop),
fd_(fd),
events_(0),
revents_(0),
index_(-1){


}
Channel::~Channel(){

    assert(!channelNoDestructor_);
}
void Channel::update(){

    loop_->update(this);
}

void Channel::remove(){

    loop_->remove(this);
}
void Channel::handlerEvent(){

    channelNoDestructor_=true;
    
    LOG_DEBUG<<"Channel::handlerEvent Events: "<<detail::getReventsName(revents_);
    if((revents_ & POLLHUP))
        if(errorCallback_)errorCallback_();

    if((revents_&POLLERR))
        if(closeCallback_)closeCallback_();

    if((revents_ &POLLIN))
        if(readCallback_)readCallback_();
    

    if((revents_&POLLOUT))
        if(writeCallback_)writeCallback_();


    channelNoDestructor_=false;

}
















}
