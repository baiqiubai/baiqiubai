
#include "LogStream.h"
#include "ResourceLimit.h"
using namespace base;

void test(){

    FixedBuffer<1> buffer;
    char c='a';
    printf("buffer.valid=%d\n",buffer.valid());
    buffer.append(&c,1);
    printf("buffer.valid %d,%s\n",buffer.valid(),buffer.data());

    buffer.append(&c,1);
    printf("buffer.valid %d,%s,size=%ld\n",buffer.valid(),buffer.data(),buffer.size());

    buffer.append(&c,1);
    printf("buffer.valid %d,%s,size=%ld\n",buffer.valid(),buffer.data(),buffer.size());
    buffer.append(&c,1);
    printf("buffer.valid %d,%s,size=%ld\n",buffer.valid(),buffer.data(),buffer.size());

    const char*msg="Buff";
    buffer.append(msg,sizeof(msg));
    printf("buffer.valid %d,%s,size=%ld\n",buffer.valid(),buffer.data(),buffer.size());

    buffer.bZero();
    printf("buffer.valid %d,%s,size=%ld\n",buffer.valid(),buffer.data(),buffer.size());


    const char*message="HELLO LINUX";
    buffer.append(message,strlen(message));
    printf("buffer.valid %d,%s,size=%ld\n",buffer.valid(),buffer.data(),buffer.size());
}
void test1(){

    FixedBuffer<8000> buffer;
    char buf[17000];
    memset(buf,'a',sizeof(buf));
    printf("buf %s\n",buf);

    buffer.append(buf,sizeof(buf));
    printf("buffer.valid %d,%s,size=%ld\n",buffer.valid(),buffer.data(),buffer.size());
    
}
void test3(){

    FixedBuffer<4000*1024> buffer;
    
    char buf[4000*1024];
    memset(buf,'a',sizeof(buf));
    buffer.append(buf,sizeof(buf));
    printf("buffer.valid %d,%s,size=%ld\n",buffer.valid(),buffer.data(),buffer.size());
}

        
int main(){

    // test();
    // test1();

    test3();
}
