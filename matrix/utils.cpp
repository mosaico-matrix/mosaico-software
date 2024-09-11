#pragma once

#include <fstream>
#include <string>
#include "logger/logger.h"

class Utils
{
public:
    /// Generic function to read file content into a string
    static bool readFile(const string& path, string &content) {

        content = "";

        std::ifstream file(path);
        std::string line;
        if (!file.is_open()) {
            Logger::logError("Could not open file: " + path);
            return false;
        }
        while (std::getline(file, line)) {
            content += line + "\n";
        }
        file.close();
        return true;
    }

};

