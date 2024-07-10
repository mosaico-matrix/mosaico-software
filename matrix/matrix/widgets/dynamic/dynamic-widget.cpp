#ifndef DYNAMIC_RUNNER_CPP
#define DYNAMIC_RUNNER_CPP

#include "../matrix-widget.h"
#include "../../drawables/drawable-shape.cpp"
#include "../../drawables/drawable-text.cpp"
#include "../../drawables/drawable.h"
#include "../../drawables/colors.cpp"
#include <utility>
#include <variant>
#include "../../../utils.cpp"
#include "dynamic-widget-metadata.cpp"
#include <pybind11/embed.h>
#include <iostream>

namespace py = pybind11;

class DynamicWidget : public MatrixWidget {

public:

    std::string widgetScriptString;
    std::string configScriptString;
    DynamicWidget(const string &widgetDirPath, const string &configDirPath) : MatrixWidget() {

        // Default fps if something goes wrong
        setFps(1);

        // Paths
        std::string widgetMetadataPath = widgetDirPath + "/mosaico.json";
        std::string widgetScriptPath = widgetDirPath + "/widget.py";
        std::string configScriptPath = configDirPath + "/config.py";

        // File contents
        std::string metadataString;
        Utils::readFile(widgetMetadataPath, metadataString);
        Utils::readFile(widgetScriptPath, widgetScriptString);
        if (!configDirPath.empty()) {
            Utils::readFile(configScriptPath, configScriptString);
        }

        // Read script metadata
        DynamicWidgetMetadata metadata;
        try {
            json j = json::parse(metadataString);
            metadata = j.template get<DynamicWidgetMetadata>();
        }
        catch (const nlohmann::json::exception &e) {
            Logger::logError("Error while parsing metadata: " + std::string(e.what()));
            return;
        }

        Logger::logDebug("Parsed dynamic runner metadata: " + widgetMetadataPath);
#ifdef DEBUG
        metadata.dump();
#endif

        // Configure script based on metadata
        setFps(metadata.fps);

        // TODO: Set canvas size

        // If we got here, the widget is valid
        validWidget = true;
        Logger::logDebug("Widget loaded successfully");
    }

    ~DynamicWidget() {
        py::finalize_interpreter();
    }

private:
    bool validWidget = false;

    void bindObjectsToPython() {
        // Get bound module
        py::module mosaico_module = py::module::import("mosaico");

        // Pass this very object to the module
        py::object widget = py::cast(this);
        mosaico_module.attr("widget") = widget;
    }

    // Loop
    bool scriptInitialized = false;
    void renderNextCanvasLayer(CanvasLayer *canvas) override {

        // Initialize Python script
        if(!scriptInitialized) {
            py::initialize_interpreter();
            bindObjectsToPython();
            try {
                //Logger::logDebug(configScriptString);
                py::exec(configScriptString);
                py::exec(widgetScriptString);
            } catch (const py::error_already_set &e) {
                Logger::logError("Error while evaluating script: " + std::string(e.what()));
                validWidget = false;
                return;
            }
            scriptInitialized = true;
        }


        // If script is invalid don't try to render
        if (!validWidget) {
            canvas->Fill(RED_COLOR);
            return;
        }


        // Execute Python loop code
        try {
            py::exec("loop()");
        } catch (const py::error_already_set &e) {
            Logger::logError("Error while evaluating loop function: " + std::string(e.what()));
            return;
        }

    }
};

#endif
