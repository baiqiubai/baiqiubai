
#include "LogStream.h"
#include "TimeStamp.h"
#include <assert.h>
using namespace  base;
using namespace std;
void test(){//整数
        
     LogStream log;

     Buffer &buff=log.getBuffer();

     log<<111;
     assert(buff.toString()==std::string("111")); 

     long long int a=10000000;
     log<<a;
     assert(buff.toString()==string("11110000000"));

     log<<-1234567890;
     assert(buff.toString()==string("11110000000-1234567890"));

     log<<33333;
     assert(buff.toString()==string("11110000000-123456789033333"));
    
     log<<0;
     assert(buff.toString()==string("11110000000-1234567890333330"));

     buff.bZero();
     double d=0.567678;
     log<<d;
     printf("buff %s buff valid %d\n",buff.data(),buff.valid());
    
     double t=-0.000000005;
     log<<t;
     printf("buff %s buff valid %d\n",buff.data(),buff.valid());


     log<<0.999999999112;
     printf("buff %s buff valid %d\n",buff.data(),buff.valid());
    
}
string Rvalue(){

    return string("C++");
}
void test1(){


    LogStream log;
    Buffer &buffer=log.getBuffer();

    const char*s="hello LogStream";

    log<<s;
    assert(buffer.toString()==string("hello LogStream"));

    const char*n=NULL;
    log<<n;
    assert(buffer.toString()==string("hello LogStream(NULL)"));
    
    const char*t=nullptr;
    log<<t;
    assert(buffer.toString()==string("hello LogStream(NULL)(NULL)"));

    buffer.bZero();

    const string name="Name";

    log<<name;
    assert(buffer.toString()==string("Name"));
    buffer.bZero();

    log<<Rvalue();
    assert(buffer.toString()==string("C++"));

    printf("%s\n",buffer.data()+5);

    buffer.bZero();
    char buf[10];
    memset(buf,'a',sizeof(buf));

    log<<buf;

    assert(buffer.toString()==string("aaaaaaaaaa"));

    
    buffer.bZero();
    char c='a';
    log<<c;

    assert(buffer.toString()==string("a"));

    std::string ss="";

    log<<ss;
    assert(buffer.toString()==string("a"));
}
void test3(int num){

    LogStream log;
    char buf[100];
    memset(buf,'a',sizeof(buf));
    TimeStamp start(TimeStamp::now());

    for(int i=0;i<num;++i){
    
           log<<buf; 
    }
    TimeStamp end(TimeStamp::now());
    printf("interval %f\n",timeDifference(end,start));
}
int main(int argc,char**argv){


    // test();
 test1();
    // test3(atoi(argv[1]));



}
