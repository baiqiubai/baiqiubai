
#include "CountDownLatch.h"
#include "CurrentThread.h"
#include <vector>
#include <thread>
#include <unistd.h>
using namespace std;

using namespace base;
using namespace base::CurrentThread;
CountDownLatch latch(10);

void threadfunc(int i){

    
    latch.countDown();
    printf("i %d CurrentThread tid %d, pid %d\n",i,CurrentThread::tid(),getpid());

}
int main(){

    std::vector<thread> vec;
    for(int i=0;i<10;++i)
        vec.emplace_back(thread(threadfunc,i));

    sleep(3);
    for(auto &it:vec)
        it.join();
    latch.wait();
    printf("Main thread CurrentThread tid %d,pid %d\n",CurrentThread::tid(),getpid());





}
