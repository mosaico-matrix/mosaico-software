#ifndef DYNAMIC_RUNNER_CPP
#define DYNAMIC_RUNNER_CPP

#include "../matrix-widget.h"
#include "modules/rest-module.cpp"
#include "../../drawables/drawable-shape.cpp"
#include "../../drawables/drawable-text.cpp"
#include "../../drawables/drawable.h"
#include "../../drawables/colors.cpp"
#include <utility>
#include <variant>
#include "../../../external/chaiscript/chaiscript.hpp"
#include "../../../utils.cpp"
#include "dynamic-widget-metadata.cpp"

using namespace chaiscript;

class DynamicWidget : public MatrixWidget {

public:

    DynamicWidget(const string& widgetDirPath, const string& configDirPath) : MatrixWidget() {

        // Default fps if something goes wrong
        setFps(1);

        // Create new ChaiScript instance
        chaiInstance = new ChaiScript();
        registerModules();
        registerGraphics();

        // Paths
        std::string widgetMetadataPath = widgetDirPath + "/mosaico.json";
        std::string widgetScriptPath = widgetDirPath + "/widget.chai";
        std::string configScriptPath = configDirPath + "/config.chai";

        // File contents
        std::string metadataString;
        Utils::readFile(widgetMetadataPath, metadataString);
        std::string widgetScriptString;
        Utils::readFile(widgetScriptPath, widgetScriptString);
        std::string configScriptString;
        if(!configDirPath.empty()) {
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

        // Try to evaluate the config script
        try {
            chaiInstance->eval(configScriptString);
        } catch (const chaiscript::exception::eval_error &e) {
            Logger::logError("Error while evaluating config script: " + std::string(e.what()));
            return;
        }

        // Try to evaluate the script
        try {
            chaiInstance->eval(widgetScriptString);
        } catch (const chaiscript::exception::eval_error &e) {
            Logger::logError("Error while evaluating script: " + std::string(e.what()));
            return;
        }

        // If we got here, the widget is valid
        validWidget = true;
        Logger::logDebug("Widget loaded successfully");
    }

    ~DynamicWidget() {
        delete chaiInstance;
    }

private:



    // Script
    ChaiScript *chaiInstance;
    bool validWidget = false;

    // Register here all the modules that will be available in the script
    void registerModules() {
        chaiInstance->add(RestModule::bootstrap());
    }


    // Loop
    void renderNextCanvasLayer(CanvasLayer *canvas) override {

        // If script is invalid don't try to render
        if(!validWidget) {
            canvas->Fill(RED_COLOR);
            return;
        }

        // Try to evaluate the loop function
        try {
            chaiInstance->eval("loop()");
        } catch (const chaiscript::exception::eval_error &e) {
            Logger::logError("Error while evaluating loop: " + std::string(e.what()));
            validWidget = false;
            return;
        }
    }


    /*
    |-----------------------------------------------------------------
    | Constructor shortcuts
    |-----------------------------------------------------------------
    */
    DrawableText *createDrawableText() {
        auto text = new DrawableText(this);
        return text;
    }

    DrawableRectangle *createDrawableRectangle() {
        auto rectangle = new DrawableRectangle(this);
        return rectangle;
    }

    void registerGraphics() {

        // COLORS
        chaiInstance->add_global(chaiscript::var(WHITE_COLOR), "WHITE_COLOR");
        chaiInstance->add_global(chaiscript::var(BLACK_COLOR), "BLACK_COLOR");
        chaiInstance->add_global(chaiscript::var(RED_COLOR), "RED_COLOR");
        chaiInstance->add_global(chaiscript::var(GREEN_COLOR), "GREEN_COLOR");
        chaiInstance->add_global(chaiscript::var(BLUE_COLOR), "BLUE_COLOR");
        chaiInstance->add_global(chaiscript::var(YELLOW_COLOR), "YELLOW_COLOR");
        chaiInstance->add_global(chaiscript::var(CYAN_COLOR), "CYAN_COLOR");
        chaiInstance->add_global(chaiscript::var(MAGENTA_COLOR), "MAGENTA_COLOR");
        chaiInstance->add(fun(&RandomColor), "RANDOM_COLOR");

        // Register basic canvas functions and bind them to the canvas object
//        chaiInstance->add(fun(&CanvasLayer::FillColor, getCanvasTemplate()), "_fillColor");
//        chaiInstance->add(fun(&CanvasLayer::Fill, getCanvasTemplate()), "_fillRGB");
        chaiInstance->add(fun(&CanvasLayer::Clear, getCanvasTemplate()), "_clear");
        chaiInstance->add(fun(&CanvasLayer::width, getCanvasTemplate()), "_canvasWidth");
        chaiInstance->add(fun(&CanvasLayer::height, getCanvasTemplate()), "_canvasHeight");

        // Register drawable functions
        chaiInstance->add(fun(&Drawable::setColor), "setColor");
        chaiInstance->add(fun(&Drawable::moveTo), "moveTo");
        chaiInstance->add(fun(&Drawable::translate), "translate");
        chaiInstance->add(fun(&Drawable::translateX), "translateX");
        chaiInstance->add(fun(&Drawable::translateY), "translateY");
        chaiInstance->add(fun(&Drawable::animateTo), "animateTo");
        chaiInstance->add(fun(&Drawable::hide), "hide");
        chaiInstance->add(fun(&Drawable::show), "show");
        chaiInstance->add(fun(&Drawable::isVisible), "isVisible");
        chaiInstance->add(fun(&Drawable::isAnimating), "isAnimating");
        chaiInstance->add(fun(&Drawable::getX), "getX");
        chaiInstance->add(fun(&Drawable::getY), "getY");

        // TEXT
        chaiInstance->add(chaiscript::base_class<Drawable, DrawableText>());
        chaiInstance->add(user_type < DrawableText > (), "DrawableText");
        chaiInstance->add(fun(&DynamicWidget::createDrawableText, this), "_DrawableText");
        chaiInstance->add(fun(&DrawableText::setText), "setText");
        chaiInstance->add(fun(&DrawableText::setFontHeight), "setFontHeight");

        // SHAPES
        chaiInstance->add(chaiscript::base_class<Drawable, DrawableShape>());

        // RECTANGLE
        chaiInstance->add(chaiscript::base_class<Drawable, DrawableRectangle>());
        chaiInstance->add(chaiscript::base_class<DrawableShape, DrawableRectangle>());
        chaiInstance->add(user_type < DrawableRectangle > (), "DrawableRectangle");
        chaiInstance->add(fun(&DynamicWidget::createDrawableRectangle, this), "_DrawableRectangle");
        chaiInstance->add(fun(&DrawableRectangle::setSize), "setSize");

//        // PPM
//        chai->add(user_type<DrawablePPM> (), "_DrawablePPM");
//        chai->add(chaiscript::base_class<Drawable, DrawablePPM>());
    }

};

#endif