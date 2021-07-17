
#include "LogFile.h"
#include "Logger.h"
#include "TimeStamp.h"

#include <unistd.h>
using namespace std;
using namespace base;

LogFile *g_file=NULL;


void ChangeOutput(const char*msg,size_t len){

    g_file->append(msg,len);
}
void test(const char*pathname,const char*filename,int num){

    LogFile::setLogPath(pathname);
    string name(filename);
    LogFile file(name,100000,8888);
    g_file=&file;

    Logger::setOutput(ChangeOutput);
    string buf(100,'a');
    TimeStamp start(TimeStamp::now());
    for(int i=0;i<num;++i){
        
        LOG_INFO<<buf<<i;
        usleep(1000);
    }
    TimeStamp end(TimeStamp::now());
    printf("interval %f\n",timeDifference(end,start));




}
void test1(const char*pathname,const char*filename,int num){
    
    LogFile::setLogPath(pathname);

    string name(filename);
    LogFile file(name,200000,500);
    g_file=&file;

    Logger::setOutput(ChangeOutput);
    string buf(100,'a');
    TimeStamp start(TimeStamp::now());
    for(int i=0;i<num;++i){
        
        LOG_INFO<<buf<<i;
        usleep(1000);
    }
    TimeStamp end(TimeStamp::now());
    printf("interval %f end Time %s\n",timeDifference(end,start),end.toFormatString(true).c_str());


}



int main(int argc,char**argv){

    if(argc==2){
        
        // test(::basename(argv[0]),atoi(argv[1]));
            
        test1("./",::basename(argv[0]),atoi(argv[1]));
    }else if(argc==4){
        
        test(argv[1],argv[2],atoi(argv[3]));
    
    }

}



