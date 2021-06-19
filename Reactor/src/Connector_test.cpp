

#include "Connector.h"
#include "EventLoop.h"
#include "InetAddress.h"
using namespace net;


void Onconnection(){

    printf("NewConnectionCallback\n");
}
int main(int argc,char **argv){


    InetAddress address(argv[1],atoi(argv[2]));
    EventLoop loop;
    
    loop.setPollInterval(-1);
    Connector connector(&loop,address);

    connector.setNewConnectionCallback(std::bind(&Onconnection));

    connector.start();

    loop.loop();




}
