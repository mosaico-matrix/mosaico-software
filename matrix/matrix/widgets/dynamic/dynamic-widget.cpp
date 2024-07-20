#ifndef DYNAMIC_RUNNER_CPP
#define DYNAMIC_RUNNER_CPP

#include "../matrix-widget.h"
#include <utility>
#include <variant>
#include "../../../utils.cpp"
#include "dynamic-widget-metadata.cpp"
#include <pybind11/embed.h>
#include <iostream>
#include "../../../external/pybind/include/pybind11_json.hpp"
#include "modules/colors-module.cpp"
namespace py = pybind11;
using namespace pybind11::literals;

class DynamicWidget : public MatrixWidget {
public:

    DynamicWidget(std::string widgetDirPath, std::string configDirPath)
            : MatrixWidget(),
              widgetDirPath(std::move(widgetDirPath)),
              configDirPath(std::move(configDirPath)),
              validWidget(false){

        setFps(1); // Default fps if something goes wrong

        if (!initializePaths() || !readMetadata()) {
            validWidget = false;
            Logger::logError("Widget initialization failed");
        } else {
            validWidget = true;
            Logger::logDebug("Widget loaded successfully");
        }
    }

    ~DynamicWidget() override {
        py::finalize_interpreter();
        Logger::logDebug("DynamicWidget destroyed");
    }

    /// Returns the path to the widget asset
    std::string widgetAssetPath(const std::string &assetName) {
        return widgetDirPath + "/assets/" + assetName;
    }

    /// Returns the path to the config asset
    std::string configAssetPath(const std::string &assetName) {
        return configDirPath.empty() ? "" : configDirPath + "/assets/" + assetName;
    }


private:
    std::string widgetScriptString;
    std::string configDataString;
    std::string widgetDirPath;
    std::string configDirPath;
    std::string widgetMetadataString;
    bool validWidget;

    bool initializePaths() {
        std::string widgetMetadataPath = widgetDirPath + "/mosaico.json";
        std::string widgetScriptPath = widgetDirPath + "/widget.py";
        std::string configScriptPath = configDirPath + "/config.json";

        return Utils::readFile(widgetMetadataPath, widgetMetadataString) &&
               Utils::readFile(widgetScriptPath, widgetScriptString) &&
               (configDirPath.empty() || Utils::readFile(configScriptPath, configDataString));
    }

    bool readMetadata() {
        try {
            json j = json::parse(widgetMetadataString);
            DynamicWidgetMetadata metadata = j.get<DynamicWidgetMetadata>();
            Logger::logDebug("Parsed dynamic runner metadata");
#ifdef DEBUG
            metadata.dump();
#endif
            setFps(metadata.fps);
            // TODO: Set canvas size based on metadata
            return true;
        } catch (const nlohmann::json::exception &e) {
            Logger::logError("Error while parsing metadata: " + std::string(e.what()));
            return false;
        }
    }

    bool scriptsInitialized = false;
    void initializeScripts() {
        try {
            py::initialize_interpreter();
            bindObjectsToPython();
            py::exec(widgetScriptString);
        } catch (const py::error_already_set &e) {
            Logger::logError("Error while initializing scripts: " + std::string(e.what()));
            validWidget = false;
        }
        scriptsInitialized = true;
    }

    void bindObjectsToPython() {

        // Take the main module
        py::module mosaico_module = py::module::import("mosaico");

        // Pass this very object to the python script
        py::object widget = py::cast(this);
        mosaico_module.attr("widget") = widget;

        // Register other modules
        ColorsModule::load(&mosaico_module);

        // Pass the config object if it exists
        if (!configDataString.empty()) {
            py::dict config = nlohmann::json::parse(configDataString);
            mosaico_module.attr("config") = config;

            // Set the working directory to the config directory
            py::exec("import os\nos.chdir('" + configDirPath + "')");
        }else
        {
            // Prevent crashes when importing the config object
            mosaico_module.attr("config") = "";

            // Set the working directory to the widget directory
            py::exec("import os\nos.chdir('" + widgetDirPath + "')");
        }
    }

    void renderNextCanvasLayer(CanvasLayer *canvas) override {

        if (!scriptsInitialized) {
            initializeScripts();
        }

        if (!validWidget) {
            canvas->Fill(RED_COLOR);
            return;
        }

        try {
            py::exec("loop()");
        } catch (const py::error_already_set &e) {
            Logger::logError("Error while executing loop function: " + std::string(e.what()));
            validWidget = false;
            canvas->Fill(RED_COLOR);
        }
    }
};

#endif
