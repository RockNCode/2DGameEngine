#ifndef LOGGER_H
#define LOGGER_H
#include <string>

class Logger {
    public:
        static void Log(const std::string& msg);
        static void Err(const std::string& msg);

};

#endif