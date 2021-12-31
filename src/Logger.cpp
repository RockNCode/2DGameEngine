#include "Logger.h"
#include <iostream>
#include <ctime>
#include <iomanip>

void Logger::printWithCurrentTimestamp(const std::string& msg,const std::string& prefix,int color) {
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    std::cout << "\033[1;"<< color <<"m"<< prefix <<": [ " << std::put_time(&tm, "%d-%b-%Y %H:%M:%S") << " ] "<< msg << std::endl;
}
void Logger::Log(const std::string& msg) {
    printWithCurrentTimestamp(msg,"LOG",32);
}
void Logger::Err(const std::string& msg) {
    printWithCurrentTimestamp(msg,"ERR",31);
}