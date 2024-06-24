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
    std::string widgetScript;         // chai
    std::string scriptPath;

    // Metadata
    DynamicRunnerMetadata metadata;
    std::string metadataPath;

    // Config
    std::string configScript;           // chai
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

    // Generic function to check if a file exists
    bool fileExists(const std::string& name) {
        return ( access( name.c_str(), F_OK ) != -1 );
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
        readFile(scriptPath, widgetScript);
        scriptLastModified = getScriptLastModified();
        Logger::logDebug("Parsed dynamic widget script file: " + scriptPath);
    }

    void loadConfig() {

        // Check if data.json exists
        string dataPath = configDirPath + "/data.json";
        if (!fileExists(dataPath)) {
            Logger::logDebug("Configuration data file not found, skipping: " + dataPath);
        }
        else
        {
            // data should already be in .chai form
        }


        Logger::logDebug("Parsed dynamic widget config file: " + configDirPath);
    }

public:

    explicit DynamicWidgetParser(const string& runnerDirPath, const string& configDirPath) {
        
        // Create file paths
        scriptPath = runnerDirPath + "/runner.chai";
        metadataPath = runnerDirPath + "/mosaico.json";

        // Load various files
        loadMetadata();
        loadScript();
        if (!configDirPath.empty()) {
            this->configDirPath = configDirPath;
            loadConfig();
        }
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
        return widgetScript;
    }

    void reloadScript()
    {
        loadScript();
    }
};

#endif