
#include "CurrentThread.h"
#include "EventLoop.h"
#include "InetAddress.h"
#include "Logger.h"
#include "TcpClient.h"
using namespace net;
using namespace base;

EventLoop *g_loop;
TcpClient *g_client;
void QuitLoop()
{
    LOG_INFO << "Quit TcpClient";
    g_client->stop();
}

void debug() { Logger::setLogLevel(Logger::LogLevel::DEBUG); }
int main(int argc, char **argv)
{
    {
        InetAddress address(argv[1], atoi(argv[2]));

        EventLoop loop;
        g_loop = &loop;
        TcpClient client(&loop, address);
        g_client = &client;

        if (argc > 3) debug();
        loop.runAfter(std::bind(&QuitLoop), 1);
        loop.runAfter(std::bind(&EventLoop::stop, &loop), 2);
        client.start();

        base::CurrentThread::sleepUsec(100 * 1000);
        loop.loop();
    }
}
