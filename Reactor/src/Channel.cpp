
#include "Channel.h"
#include "EventLoop.h"

#include <assert.h>
namespace net {

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
    if((revents_&POLLERR))
        if(closeCallback_)closeCallback_();

    if((revents_ &POLLIN))
        if(readCallback_)readCallback_();
    

    if((revents_&POLLOUT))
        if(writeCallback_)writeCallback_();

    channelNoDestructor_=false;

}
















}
