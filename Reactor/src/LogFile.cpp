
#include "LogFile.h"
#include "CurrentThread.h"
#include "Logger.h"
#include "LogStream.h"

#include <dirent.h>
#include <time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
namespace base {

namespace detail{

    __thread time_t t_lastTime;
    __thread char t_Time[64]={0};
    __thread char hostName[100]={0};
    __thread bool hasHostName=false;

    char* HostName(){
        
        if(::gethostname(hostName,sizeof(hostName))){
        
            
        }
        hasHostName=true;
        return hostName;
    }
    

}
    LogFile::LogFile(const std::string &basename,
                size_t fileSize,
                size_t fileRows,
                size_t flushInterval,
                bool threadSafe)
    :baseName_(basename),
    fileSize_(fileSize),
    fileRows_(fileRows),
    flushInterval_(flushInterval),
    mutex_(threadSafe? new MutexLock():NULL),
    count_(0)
    {
    
        rollFile();
    
    
    }
   void LogFile::rollFile(){
     
        time_t time;

        std::string basename=getBaseName(&time);

        // int64_t seconds=time/ 1000000;
    
        time_t seconds=time;
        if(seconds !=lastRollFile_){
            lastRollFile_=seconds;
            lastFlush_=seconds;
            file_.reset(new FileUtil(basename));
        }
   
   } 
   void LogFile::flush(){
    
       if(mutex_){
            MutexLockGuard lock(*mutex_);
            file_->flush();
       }else {
        
           file_->flush();
       }
     
   } 
   std::string LogFile::getBaseName(time_t *now){
   
        std::string filename;


        *now=time(NULL);
       
        int64_t Seconds=*now;
        time_t seconds=static_cast<time_t>(*now / 1000000);

        int microseconds=*now %1000000;

        
        if(seconds!=detail::t_lastTime){
            struct tm tm_time;
            
            ::gmtime_r(&Seconds,&tm_time);
            snprintf(detail::t_Time,
                    sizeof(detail::t_Time),
                    "%4d%02d%02d-%02d:%02d:%02d",
                    tm_time.tm_year+1900,
                    tm_time.tm_mon+1,
                    tm_time.tm_mday,
                    tm_time.tm_hour,
                    tm_time.tm_min,
                    tm_time.tm_sec);
            detail::t_lastTime=seconds;
            
        }

        char buf[20]={0};
        snprintf(buf,sizeof(buf),"%06d",microseconds);

        filename+=detail::t_Time;
        filename+=".";
        filename+=buf;

        
        if(!detail::hasHostName){
        
            detail::HostName();
        }
        filename+=detail::hostName;
        filename+=".";    
        char f[10]={0};
        snprintf(f,sizeof(buf),"%d",CurrentThread::tid());
        filename+=f;


        filename+=".log";

        return filename;


   
   }
   void LogFile::write(const char*msg,size_t len){
        
        file_->append(msg,len);
        if(file_->nwrittenBytes() > fileSize_){
        
            rollFile();
        }else {
            count_++;


            if(static_cast<size_t>(count_) >= fileRows_){
                count_=0;                       
                rollFile();          
            }else{
                time_t time=::time(NULL);
                
            
            }
             
        
        }
        

   }
   void LogFile::append(const char*msg,size_t len){
   

            if(mutex_){
            
                MutexLockGuard lock(*mutex_);
                write(msg,len);
            }else {
            
                write(msg,len);
            }
          
   
   
   }
    
    void LogFile::setLogPath(const char*filename){
    
        struct stat buf; 
        if(::stat(filename,&buf)){
            LOG_ERROR<<"stat error";        
        } 
        if(S_ISDIR(buf.st_mode)){
            
            DIR* dir=opendir(filename);
            if(dir ==NULL){
                LOG_ERROR<<"opendir error";
            }else {
                if(::chdir(filename)){
                    LOG_ERROR<<"chdir error";
                }
            }
        }else if(S_ISREG(buf.st_mode)){
                
            printf("Not Dir\n");     
        }else {
            
        }
    }

} 
