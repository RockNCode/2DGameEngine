#include "Logger.h"
#include <iostream>
#include <ctime>
#include <iomanip>

void Logger::Log(const std::string& msg) {
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    
    std::cout << "\033[0;32mLOG: [ " << std::put_time(&tm, "%d-%b-%Y %H:%M:%S") << " ] "<< msg << std::endl;

}
void Logger::Err(const std::string& msg) {
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::cerr << "\033[0;33mERR: [ " << std::put_time(&tm, "%d-%b-%Y %H:%M:%S") << " ] "<< msg << std::endl;
}