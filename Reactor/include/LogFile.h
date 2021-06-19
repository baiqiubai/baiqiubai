
#ifndef LOGFILE_BASE_H_
#define LOGFILE_BASE_H_

#include "Mutex.h"
#include "FileUtil.h"
#include <memory>
namespace  base {

namespace detail{

    extern __thread time_t t_lastTime;
    extern __thread char t_Time[64];
    extern __thread char hostName[100];
    extern __thread bool hasHostName;
    char *HostName();
}
class LogFile{

    public:
        LogFile(const std::string &basename,
                size_t fileSize=2000000,
                size_t fileRows=8888,
                size_t flushInterval=3,
                bool threadSafe=false);

        ~LogFile()=default;

        void flush();
        void append(const char*msg,size_t len);

        static void setLogPath(const char*filename);
    private:

        void write(const char*msg,size_t len);
        void rollFile();
        std::string getBaseName(time_t *now);
        const std::string baseName_;
        
        const size_t fileSize_;

        const size_t fileRows_;

        const size_t flushInterval_;

        std::unique_ptr<MutexLock> mutex_;
        
        
        int count_; //

        time_t startOfPeriod_;
        time_t lastFlush_;
        time_t lastRollFile_;
        std::unique_ptr<FileUtil> file_;        

        static const int kSecondsToDay=3600*24;
};



}


#endif 
