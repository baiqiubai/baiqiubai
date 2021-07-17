
#include "EventLoopThreadPool.h"
#include "EventLoop.h"
#include "CurrentThread.h"

#include <assert.h>
using namespace net;
using namespace base;
using namespace std;
EventLoop *g_loop;
void print(){

    // g_loop->runInLoop(std::bind(&EventLoop::stop,g_loop));
}
void test(){
    
    EventLoop eventLoop;
    EventLoopThreadPool pool(&eventLoop);
    pool.setThreadNum(3);
    pool.start();

    EventLoop *baseLoop_=pool.getBaseLoop();
    EventLoop *loop=pool.getNextLoop();
    g_loop=loop;
    
    assert(baseLoop_!=loop);
    EventLoop *loop1=pool.getNextLoop();
    EventLoop *loop2=pool.getNextLoop();
    EventLoop *loop3=pool.getNextLoop();
    assert(loop3==loop);


}
int main(){


    test();



}
