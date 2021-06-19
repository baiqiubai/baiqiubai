
#include "TcpClient.h"
#include "InetAddress.h"
#include "EventLoop.h"
using namespace net;
using namespace std;

int main(int argc,char**argv){


    InetAddress address(argv[1],atoi(argv[2]));
    
    EventLoop loop;
    TcpClient client(&loop,address);

    client.start();
    loop.loop();




}
