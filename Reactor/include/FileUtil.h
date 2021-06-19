
#ifndef FILEUTIL_BASE_H_
#define FILEUTIL_BASE_H_

#include <iostream>
namespace base {

class FileUtil{
    public:
    
        FileUtil(const std::string &filename);
        ~FileUtil();

        void append(const char*msg,size_t len);
        
    
        void flush();
        size_t nwrittenBytes()const{return nwrittenBytes_;}
    private:

        void write_unlocked(const char*msg,size_t len);
        FILE *fp_;
        size_t nwrittenBytes_;
        char buffer_[4000*1024];
};





} 

#endif 
