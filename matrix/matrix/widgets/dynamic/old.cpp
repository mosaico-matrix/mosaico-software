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



        // Generic function to check if a file exists
        bool fileExists(const std::string& name) {
            return ( access( name.c_str(), F_OK ) != -1 );
        }

        void loadMetadata() {
            string metadataString;
            readFile(metadataPath, metadataString);
            json j = json::parse(metadataString);
            std::cout << metadataString << std::endl;
            metadata =  j.template get<DynamicWidgetMetadata>();
            Logger::logDebug("Parsed dynamic runner metadata: " + metadataPath);
#ifdef DEBUG
            metadata.dump();
#endif
        }

        void loadScript() {
            readFile(scriptPath, widgetScript);
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


            // Load various files
            loadMetadata();
            loadScript();
            if (!configDirPath.empty()) {
                this->configDirPath = configDirPath;
                loadConfig();
            }
        }

};

#endif