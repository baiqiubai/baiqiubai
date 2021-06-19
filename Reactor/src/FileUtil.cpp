

#include "FileUtil.h"
#include "Logger.h"
#include "LogStream.h"

#include <cstdio>
namespace base {

    FileUtil::FileUtil(const std::string &filename)
    :fp_(fopen(filename.c_str(),"a+")),
    nwrittenBytes_(0)
    {
    
            
        if(!fp_) LOG_ERROR<<"fopen error";
        
        ::setbuffer(fp_,buffer_,sizeof(buffer_));
    
    }
    void FileUtil::flush(){
    
         if(::fflush(fp_)){
         
                LOG_ERROR<<"fflush error"; 
         }   

    }
    FileUtil::~FileUtil(){
    
        if(::fclose(fp_)){
            
            LOG_ERROR<<"flose error";
        }

    }
    void FileUtil::append(const char*msg,size_t len){
    
    
        write_unlocked(msg,len);
    
    }

    void FileUtil::write_unlocked(const char*msg,size_t len){
    
    
        const char*ptr=msg;
        size_t nwritten=len;

        while(nwritten){
        
            
            size_t nbytes=::fwrite_unlocked(ptr,1,nwritten,fp_);
            if( nbytes <0){
            
            
            }
            
            ptr+=nbytes;
            nwritten-=nbytes;



        
        
        
        }
        nwrittenBytes_+=len;
    
    
    
    
    }









}
