
#include "Buffer.h"

#include <cstring>
using namespace std;
using namespace net;

void test(){

    Buffer buffer;
    char data[500];
    memset(data,'a',sizeof(data));
    buffer.BufferAdd(sizeof(data),data);
//

    printf("buffer totalSize %ld\n",buffer.totalSize());
    memset(data,'c',sizeof(data));
    buffer.BufferAdd(500,data);
    // buffer.foreachWithDataChain();
    printf("buffer totalSize %ld\n",buffer.totalSize());

    int cnt=10;
    
}
int main(){


    test();


}


