
#include "Buffer.h"

#include <cstdio>
#include <cstring>
using namespace net;

void test(){

    Buffer buffer;
    printf("writeable capacity %ld %ld\n",buffer.writeAbleSpace(),buffer.capacity());
    char buf[1024];
    memset(buf,'a',sizeof(buf));
    buffer.append(buf,sizeof(buf));
    
    char b[10];
    buffer.append(b,sizeof(b));
    printf("writeable capacity %ld %ld\n",buffer.writeAbleSpace(),buffer.capacity());

}

int main(){

    test();
}
