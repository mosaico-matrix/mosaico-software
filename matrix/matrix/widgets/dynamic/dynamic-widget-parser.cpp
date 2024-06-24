#ifndef DYANMIC_RUNNER_PARSER_CPP
#define DYANMIC_RUNNER_PARSER_CPP

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <list>
#include <unordered_map>
#include "../../../logger/logger.h"
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include "../../../configs.cpp"
#include "../../../external/json/json.hpp"
#include "dynamic-widget-metadata.cpp"

using json = nlohmann::json;
using namespace std;

class DynamicWidgetParser {

private:
    std::string runnerDirPath;

    // Script
    std::string script;
    std::string scriptPath;
    time_t scriptLastModified;

    // Metadata
    DynamicRunnerMetadata metadata;
    std::string metadataPath;

    // Config
    std::string configDirPath;

    // Generic function to read file content into a string
    void readFile(string path, string &content) {

        content = "";

        std::ifstream file(path);
        std::string line;
        if (!file.is_open()) {
            Logger::logFatal("Could not open file: " + path);
            return;
        }
        while (std::getline(file, line)) {
            content += line + "\n";
        }
        file.close();
    }

    time_t getScriptLastModified() {
        struct stat fileInfo;
        if (stat(scriptPath.c_str(), &fileInfo) != 0) {
            Logger::logError("Could not get file info for dynamic runner file: " + scriptPath);
        }
        return fileInfo.st_mtime;
    }

    void loadMetadata() {
        string metadataString;
        readFile(metadataPath, metadataString);
        json j = json::parse(metadataString);
        std::cout << metadataString << std::endl;
        metadata =  j.template get<DynamicRunnerMetadata>();
        Logger::logDebug("Parsed dynamic runner metadata: " + metadataPath);
#ifdef DEBUG
        metadata.dump();
#endif
    }

    void loadScript() {
        readFile(scriptPath, script);
        scriptLastModified = getScriptLastModified();
        Logger::logDebug("Parsed dynamic runner file: " + scriptPath);
    }

public:

    explicit DynamicWidgetParser(const string& runnerDirPath, const string& configDirPath) {
        
        // Get path to the runner
        scriptPath = runnerDirPath + "/runner.chai";
        metadataPath = runnerDirPath + "/mosaico.json";

        // Load various files
        loadMetadata();
        loadScript();
    }

    bool scriptChanged() {
        time_t lastModified = getScriptLastModified();
        if (scriptLastModified != lastModified) {
            scriptLastModified = lastModified;
            return true;
        }
        return false;
    }

    int getFrameRate() {
        return metadata.fps;
    }

    std::string getScript() {
        return script;
    }

    void reloadScript()
    {
        loadScript();
    }
};

#endif