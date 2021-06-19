
#include "Logger.h"
#include <unistd.h>
using namespace std;
using namespace base;

FILE *fp;
void ChangeOutput(const char*msg,size_t len){
    
    if(fp){
        
        ::fwrite(msg,1,len,fp);
    }else {
    
        ::fwrite(msg,1,len,stdout);
    }
    
}
void ChangeFlush(){
    if(fp){
        
        ::fflush(stdout);

    }else {
        ::fflush(fp);
    }
    
}

void test (const char* type,int num){

    Logger::setLogLevel(Logger::LogLevel::TRACE);
    Logger::setOutput(ChangeOutput);
    Logger::setFlush(ChangeFlush);

    fp=fopen(type,"w");
    char buffer[64*1024];
    setbuffer(fp,buffer,sizeof(buffer));
    TimeStamp start(TimeStamp::now());
    
    // char buf[1000];
    std::string buf(3000,'a');

    for(int i=0;i<num;++i){
        
        LOG_INFO<<"hello world";
    }

    TimeStamp end(TimeStamp::now());
    double seconds=timeDifference(end,start);
    printf("%s---interval %f,%12.2f msg/s\n",type,seconds,
            num / seconds);

}
int main(int argc,char**argv){
    
    // char *type="Logger.txt";
    int num=atoi(argv[1]);
        
    // test(type,num);
    // bool empty=atoi(argv[2]) > 0;
    const char *ttype="/dev/null";
    test(ttype,num);

}
