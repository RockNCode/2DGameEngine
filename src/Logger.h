#ifndef LOGGER_H
#define LOGGER_H
#include <string>

class Logger {
    private:
        static void printWithCurrentTimestamp(const std::string& msg,const std::string& prefix,int color);
    public:
        static void Log(const std::string& msg);
        static void Err(const std::string& msg);

};

#endif