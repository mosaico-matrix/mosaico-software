//
// Created by Marco Coppola on 02/03/24.
//

#ifndef MAGIC_SQUARE_SW_LOGGER_H
#define MAGIC_SQUARE_SW_LOGGER_H

#include <string>
using namespace std;
class Logger {
    public:
        static void initLogger();
        static void logDebug(const std::string& msg);
        static void logInfo(const std::string& msg);
        static void logError(const std::string& msg);
        static void logFatal(const std::string& msg);
};


#endif //MAGIC_SQUARE_SW_LOGGER_H
