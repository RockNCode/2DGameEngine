#include "Logger.h"
#include <iostream>
#include <ctime>
#include <chrono>

std::vector<LogEntry> Logger::messages;

void Logger::printWithCurrentTimestamp(const std::string& msg,LogType type) {
    std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::string prefix ;
    int color;
    switch (type)
    {
    case LOG_INFO:
        prefix = "LOG";
        color = 32;
        break;
    case LOG_ERROR:
        prefix = "ERR";
        color = 31;
        break;
    case LOG_WARNING:
        prefix = "WARNING";
        color = 33;
        break;

    default:
        break;
    }

    std::string timeOut(30,'\0');
    std::strftime (&timeOut[0],timeOut.size(),"%d-%b-%Y %H:%M:%S",std::localtime(&now));
    LogEntry logEntry;
    logEntry.type = LOG_INFO;
    logEntry.message = prefix+": [ "+timeOut+" ] " + msg;

    std::cout << "\033[1;"<< color <<"m"<< logEntry.message << "\033[0m" <<std::endl;

    messages.push_back(logEntry);

}
void Logger::Log(const std::string& msg) {
    printWithCurrentTimestamp(msg,LOG_INFO);
}
void Logger::Err(const std::string& msg) {
    printWithCurrentTimestamp(msg,LOG_ERROR);
}
void Logger::Warn(const std::string& msg) {
    printWithCurrentTimestamp(msg,LOG_WARNING);
}