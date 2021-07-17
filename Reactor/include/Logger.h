
#ifndef LOGGER_BASE_H_
#define LOGGER_BASE_H_
#include <cstring>

#include "TimeStamp.h"
#include "LogStream.h"
namespace base {
extern __thread time_t t_lastTime;
extern __thread char   t_Seconds[32];
class Logger {
public:
    enum LogLevel {
        TRACE,
        DEBUG,
        INFO,
        WARN,
        ERROR,
        SYSFATAL,
        LOG_NUMS
    };
    struct SourceFile {
    
        SourceFile(const char* filename)
        {
            const char* t = strrchr(filename, '/');
            if (t) {
                file_ = t + 1;
            }
            else {
                file_ = filename;
            }
            size_=static_cast<size_t>(strlen(file_));
        }

        const char* file_;
        size_t size_;
    };

    Logger(SourceFile file,Logger::LogLevel level,int line,const char*func);
    Logger(SourceFile file,Logger::LogLevel level,int line);
    
    LogStream& getBuffer(){return stream_;}
    using OutputFunc=void(*)(const char*msg,size_t len);
    using FlushFunc=void(*)();
    static void setOutput(OutputFunc func);
    static void setFlush(FlushFunc func);
    static void setLogLevel(Logger::LogLevel level);
   
    static Logger::LogLevel nowLogLevel();
    ~Logger();

    
private:
    
    void toFormatTime();
    void finish();
    SourceFile fileName_;
    Logger::LogLevel LogLevel_;
    int line_;
    const char*func_;
    TimeStamp nowTime_;
    LogStream stream_;
};
extern Logger::LogLevel g_level;
inline Logger::LogLevel Logger::nowLogLevel(){
    
    return g_level;
}

#define LOG_TRACE if( Logger::nowLogLevel()<= Logger::LogLevel::TRACE ) \
        Logger(__FILE__,Logger::LogLevel::TRACE,__LINE__).getBuffer()
#define LOG_DEBUG if(Logger::nowLogLevel()<=Logger::LogLevel::DEBUG) \
        Logger(__FILE__,Logger::LogLevel::DEBUG,__LINE__).getBuffer()
#define LOG_INFO Logger(__FILE__,Logger::LogLevel::INFO,__LINE__).getBuffer()
#define LOG_WARN Logger(__FILE__,Logger::LogLevel::WARN,__LINE__,__func__).getBuffer()
#define LOG_ERROR Logger(__FILE__,Logger::LogLevel::ERROR,__LINE__,__func__).getBuffer()
#define LOG_SYSFATAL Logger(__FILE__,Logger::LogLevel::SYSFATAL,__LINE__,__func__).getBuffer()

}  // namespace base

#endif

