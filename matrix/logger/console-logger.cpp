#include "logger.h"
#include <iostream>

// Define color codes
constexpr const char* RESET = "\033[0m";
constexpr const char* RED = "\033[31m";
constexpr const char* GREEN = "\033[32m";
constexpr const char* YELLOW = "\033[33m";
constexpr const char* BLUE = "\033[34m";

void outputColoredMsg(const std::string& level, const std::string& msg, const char* color) {
    std::cout << color << level << RESET << " " << msg << std::endl;
}


void Logger::initLogger() {
    // Do nothing for now
}

void Logger::logDebug(const std::string& msg) {
    outputColoredMsg("[DEBUG]", msg, BLUE);
}

void Logger::logInfo(const std::string& msg) {
    outputColoredMsg("[INFO]", msg, GREEN);
}

void Logger::logError(const std::string& msg) {
    outputColoredMsg("[ERROR]", msg, RED);
}

void Logger::logFatal(const std::string& msg) {
    outputColoredMsg("[FATAL]", msg, RED);
    exit(1);
}