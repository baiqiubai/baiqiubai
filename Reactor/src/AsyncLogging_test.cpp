
#include "AsyncLogging.h"
#include <unistd.h>
#include "LogStream.h"
#include "Logger.h"
#include "TimeStamp.h"

#include <sys/resource.h>
using namespace base;

using namespace std;

AsyncLogging *g_async;
void ChangeOutput(const char *msg, size_t len) { g_async->append(msg, len); }
void test(const char *filename, int num)
{
    // LogFile::setLogPath(pathname);
    // LogFile file(name,100000,8888);
    // g_file=&file;

    Logger::setOutput(ChangeOutput);
    // string buf(100, 'a');
    string buf(3000,'a');
    for (int i = 0; i < 30; ++i) {
        TimeStamp start(TimeStamp::now());
        for (int i = 0; i < num; ++i) {
            LOG_INFO << buf << i;
            // usleep(1000);
        }
        TimeStamp end(TimeStamp::now());
        printf("interval %f\n", timeDifference(end, start) * 1000000 / num);
        usleep(1000);
    }
}

int main(int argc, char **argv)
{
    {
        size_t GB=1000*1024*1024;
        struct rlimit r1;
        r1={10*GB,10*GB};
        setrlimit(RLIMIT_AS,&r1);
        
    }
    string name(::basename(argv[0]));
    AsyncLogging file(name, 200000, 500);
    file.start();
    g_async = &file;
    // test(argv[1],atoi(argv[2]));
    test(::basename(argv[0]), atoi(argv[1]));
}
