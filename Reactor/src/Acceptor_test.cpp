
#include "Acceptor.h"
#include "EventLoop.h"
#include "InetAddress.h"
using namespace std;

using namespace net;

void OnConnection(int sockfd){
    printf("New OnConnection\n");

}
int main(int argc,char **argv){

    InetAddress address(argv[1],atoi(argv[2]));

    EventLoop loop;
    Acceptor acceptor(&loop,address);

    acceptor.setNewConnectionCallback(std::bind(&OnConnection,_1));
    acceptor.listen();
    
    loop.loop();

}
