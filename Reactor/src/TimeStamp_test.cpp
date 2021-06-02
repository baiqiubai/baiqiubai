
#include "TimeStamp.h"
#include <sys/time.h>
#include <unistd.h>
using namespace base;
using namespace std;


void test(){

    for(int i=0;i<10;++i){
        TimeStamp now(TimeStamp::now());
        cout<<"Now "<<now.toFormatString(true)<<"\n";
        sleep(1);
        TimeStamp end(TimeStamp::now());
        cout<<"End "<<end.toFormatString(true)<<"\n";
    
    }

}
int main(){

    TimeStamp now(TimeStamp::now());

    struct timeval val;
    ::gettimeofday(&val,NULL);

    cout<<"gettimeofday Time"<<val.tv_sec*TimeStamp::kSecondsToMicroSeconds+val.tv_usec<<"\n";
    cout<<"now "<<now.microSeconds()<<" "<<now.toString()<<"\n";

    cout<<"Now time "<<now.toFormatString(true)<<"\n";
    
    test();

}
