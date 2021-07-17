
#include "Buffer.h"

#include <cstdio>
#include <cstring>
using namespace net;

void test(){

    printf("test\n");
    Buffer buffer;
    
    char buf[1000];
    memset(buf,'a',sizeof(buf));
    buffer.bufferAdd(sizeof(buf),buf);
    printf("totalSize %ld\n",buffer.totalSize());
}
int main(){

    test();
}
