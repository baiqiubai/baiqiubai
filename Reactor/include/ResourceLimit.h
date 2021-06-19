

#ifndef RESOURCELIMIT_BASE_H_
#define RESOURCELIMIT_BASE_H_
#include <sys/resource.h>
#include <cstdio>
namespace Limit{

unsigned long stackSizeSoft(){

    struct rlimit rlim;
    if(::getrlimit(RLIMIT_STACK,&rlim)){
        perror("getrlimit error");
    }
    return rlim.rlim_cur;
}



}

#endif 
