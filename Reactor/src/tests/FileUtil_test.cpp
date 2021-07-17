
#include "FileUtil.h"
#include "Logger.h"
#include "TimeStamp.h"
using namespace base;
using namespace std;

FileUtil *g_file;
void ChangeOutput(const char*msg,size_t len){

    
    g_file->append(msg,len);

}

void test(const char*filename,int num){


    Logger::setOutput(ChangeOutput);
    string name(filename);

    FileUtil file(name);

    g_file=&file;

    // string buf(1000,'a');
    string buf("HELLO ");
    TimeStamp start(TimeStamp::now());

    for(int i=0;i<num ;++i){
    
        LOG_INFO<<buf<<i;
    }
    TimeStamp end(TimeStamp::now());
    
    printf("interval %f\n",timeDifference(start,end));





}
int main(int argc,char**argv){


    if(argc==3){
        
        test(argv[1],atoi(argv[2]));
        
    }else{
        const char*filename="Mmap.txt";
        test(filename,atoi(argv[1]));
    }




}
