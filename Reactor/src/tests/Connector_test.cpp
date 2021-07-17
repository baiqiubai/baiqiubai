

#include "Connector.h"
#include "EventLoop.h"
#include "InetAddress.h"
#include "Logger.h"
#include "LogStream.h"

#include "TimeStamp.h"
using namespace base;
using namespace net;

Connector *g_connector;
EventLoop *g_loop ; 
void Onconnection(int sockfd){

    printf("NewConnectionCallback\n");
}
void debug(){

    base::Logger::setLogLevel(Logger::LogLevel::DEBUG);
}
int main(int argc,char **argv){


    InetAddress address(argv[1],atoi(argv[2]));
    EventLoop loop;
    g_loop=&loop;
    loop.setPollInterval(-1);
    Connector connector(&loop,address);
    g_connector=&connector;
    
    // g_connector->stop();
    loop.runAfter(std::bind(&Connector::stop,&connector),1); //10秒后连接不上退出
    loop.runAfter(std::bind(&EventLoop::stop,&loop),2); 
    connector.setNewConnectionCallback(std::bind(&Onconnection,_1));
    
    debug();
    TimeStamp start(TimeStamp::now());
    connector.start();

    loop.loop();
    TimeStamp end(TimeStamp::now());

    printf("After %f seconds Quit\n",timeDifference(start,end));





}
