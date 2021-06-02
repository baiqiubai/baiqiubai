
#include "Logger.h"
#include "CurrentThread.h"

#include <stdlib.h>
#include <time.h>
namespace base {

__thread time_t t_lastTime=0;
__thread char t_Seconds[32];
const char *LogLevel[Logger::LogLevel::LOG_NUMS]
={

    "LOG_TRACE",
    "LOG_DEBUG",
    "LOG_INFO",
    "LOG_WARN",
    "LOG_ERROR",
    "LOG_SYSFATAL"

};
static Logger::LogLevel initLogLevel(){
    
    if(::getenv("MY_LOG_TRACE")){
    
        return Logger::LogLevel::TRACE;
    }else if(::getenv("MY_LOG_DEBUG")){
        
        return Logger::LogLevel::DEBUG;
    }else {
    
        return Logger::LogLevel::INFO;
    }
    

}
Logger::Logger(SourceFile file,
        Logger::LogLevel level,
        int line,
        const char*func)
:fileName_(file),
LogLevel_(level),
line_(line),
func_(func),
nowTime_(TimeStamp::now()){

    
    toFormatTime();
    stream_<<CurrentThread::tid()<<" ";
            

}
Logger::Logger(SourceFile file,
        Logger::LogLevel level,
        int line)
:fileName_(file),
LogLevel_(level),
line_(line),
func_(NULL),
nowTime_(TimeStamp::now()){
    toFormatTime();
    stream_<<CurrentThread::tid()<<" ";



}
void Logger::toFormatTime(){
    
    
    int64_t microSeconds=nowTime_.microSeconds();
    time_t seconds=static_cast<time_t>(microSeconds/TimeStamp::kSecondsToMicroSeconds);
    int microseconds=microSeconds%TimeStamp::kSecondsToMicroSeconds;
    struct tm tm_time;

    if(seconds!=t_lastTime){
        ::gmtime_r(&seconds,&tm_time);
        snprintf(t_Seconds,sizeof(t_Seconds),
                "%04d%02d%02d-%02d:%02d:%02d",
                tm_time.tm_year+1900,tm_time.tm_mon+1,tm_time.tm_mday,
                tm_time.tm_hour,tm_time.tm_min,tm_time.tm_sec);    
    
        t_lastTime=seconds;
    }
    char buf[30]={0};
    snprintf(buf,sizeof(buf),"%06d",microseconds);
    
    stream_<<t_Seconds<<"."<<buf<<" ";    

    

}
void DefaultOutPut(const char*msg,size_t len){

    ::fwrite(msg,1,len,stdout);

}
void DefaultFlush(){

    ::fflush(stdout);
}
Logger::OutputFunc g_output=DefaultOutPut;
Logger::FlushFunc g_flush=DefaultFlush;

Logger::LogLevel g_level=initLogLevel();

// Logger::LogLevel Logger::nowLogLevel(){

    // return g_level;
// }
    
inline LogStream& operator<<(LogStream &log,Logger::SourceFile file){

    log.append(file.file_,file.size_);
    return log;
}

void Logger::finish(){

    
    stream_<<" - "<<fileName_<<" "<<line_<<" ";
    if(func_){
        
        stream_<<func_<<"\n";
    }else {
    
        stream_<<"\n";
    }

}
Logger::~Logger(){
    
    finish();
    Buffer &buffer=stream_.getBuffer();  
    g_output(buffer.data(),buffer.dataSize());
    if(LogLevel_==Logger::LogLevel::SYSFATAL){
    
        g_flush();
        abort();
    }

}


void Logger::setFlush(FlushFunc func){

    g_flush=func;
}
void Logger::setOutput(OutputFunc func){
    
    g_output=func;
}
void Logger::setLogLevel(Logger::LogLevel level){

    g_level=level;
}

}
