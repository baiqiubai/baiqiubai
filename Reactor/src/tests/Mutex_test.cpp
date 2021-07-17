
#include "Mutex.h"
#include "Condition.h"
#include <unistd.h>
#include <iostream>
#include <thread>
#include <vector>
#include <deque>
using namespace  base;

using namespace std;
MutexLock mutex_;

deque<int> dq;
Condition cond(mutex_);
int cnt=0;

void threadfunc(){

    MutexLockGuard lock(mutex_);
    for(int i=0;i<10000;++i){
        ++cnt;
    }
    
}
void test(){

    std::vector<std::thread> vec;

    for(int i=0;i<10;++i){
        vec.emplace_back(std::thread(threadfunc));
    
    }

    sleep(1);
    for(auto &it:vec)
        it.join();

    std::cout<<"cnt "<<cnt<<"\n";
}

void consumer(){

    while(1){
    
        while(dq.empty()){
        
            cond.wait();
        } 
        int x=dq.front();
        dq.pop_front();
        printf("consumer x--%d\n",x);
        sleep(3);
    }
}
void productor(){
    while(1){
    
        int x=rand()%10;
        printf("productor x--%d\n",x);
        dq.emplace_back(x);
        cond.signal();
        sleep(1);
    }

}
int main(){

    std::vector<thread> vec;

    for(int i=0;i<3;++i){
        
        if(i!=2)vec.emplace_back(thread(productor));
        else vec.emplace_back(thread(consumer));

    }

    sleep(4);
    for(auto &it:vec)
        it.join();

}
