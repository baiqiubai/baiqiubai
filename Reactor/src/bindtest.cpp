
#include <iostream>
#include <vector>
#include <functional>
#include <memory>
using namespace std;

using Functor=std::function<void()>;
using std::placeholders::_1;
class B{

    public:
        void print(){
            printf("B\n");
        }
    private:
};
class A{

    public:
        void push(const shared_ptr<B> &b){
            
            // vec.push_back(std::bind(&B::print,&b));
            queueInLoop(std::bind(&B::print,b));
        }           
        void pushPtr(const  B *b){
            
            queueInLoop(std::bind(&B::print,b));
        }
        void queueInLoop(const Functor& func){
            vec.push_back(func);
        }
        void todo(){
        
            for(auto &it:vec)it();
        }
    private:
        std::vector<Functor> vec;
};
int main(){

    A a;
    shared_ptr<B> b(new B());
    a.push(b);
    a.todo();


}
