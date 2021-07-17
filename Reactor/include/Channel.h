
#ifndef CHANNEL_NET_H_
#define CHANNEL_NET_H_

#include "Callback.h"

#include <sys/poll.h>
namespace net {

    const int kRead=POLLIN|POLLERR;
    const int kWrite=POLLOUT;
    const int kNone=0;
class EventLoop;
class Channel{

    public:
 
        Channel(EventLoop *loop,int fd);
        ~Channel();
        using Functor=std::function<void()>;
        void setWriteCallback(Functor cb){writeCallback_=std::move(cb);}
        void setReadCallback(Functor cb){readCallback_=std::move(cb);}
        void setCloseCallback(Functor cb){closeCallback_=std::move(cb);}
        void setErrorCallback(Functor cb){errorCallback_=std::move(cb);}


        void enableReading(){events_|=kRead;update();}
        void enableWriting(){events_|=kWrite;update();}
        
        bool isWriting(){return events_& kWrite;}

        void disableRead(){events_&=~kRead;update();}
        void disableWrite(){events_&=~kWrite;update();}
        void disableAll(){events_=kNone;update()   ;}
        

        bool isNoneEvent(){return events_==kNone;}
        

        void setIndex(int index){index_=index;}
    
        void setRevents(int revents){revents_=revents;}
        int index()const{return index_;}
        int fd()const{return fd_;}

        int events()const{return events_;}

        int revents()const{return revents_;}


        void handlerEvent();

        void remove();

    private:
        
        void update();
        EventLoop *loop_;
        const int fd_;

        int events_;

        int revents_;
    
        int index_; //
        readCallback readCallback_;
        writeCallback writeCallback_;
        closeCallback closeCallback_;
        errorCallback errorCallback_;
        bool channelNoDestructor_;
};







}
#endif 
