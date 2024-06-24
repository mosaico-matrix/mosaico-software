#ifndef DYNAMIC_RUNNER_CPP
#define DYNAMIC_RUNNER_CPP

#include "../matrix-widget.h"
#include "modules/rest-module.cpp"
#include "dynamic-widget-parser.cpp"
#include "../../drawables/drawable-shape.cpp"
#include "../../drawables/drawable-text.cpp"
#include "../../drawables/drawable.h"
#include "../../drawables/colors.cpp"
#include <variant>
#include "../../../external/chaiscript/chaiscript.hpp"

using namespace chaiscript;

class DynamicWidget : public MatrixWidget {

public:

    DynamicWidget(int framesPerSecond, CanvasLayerPosition position, DynamicWidgetParser *parser) : MatrixWidget(
            framesPerSecond, position) {
        this->parser = parser;
        initEnvironmentAndLoadScript();
    }

    ~DynamicWidget() {
        delete chai;
        delete parser;
    }

private:

    ChaiScript *chai;
    DynamicWidgetParser *parser;
    bool validScript = false;

    // Register here all the modules that will be available in the script
    void registerModules() {
        chai->add(RestModule::bootstrap());
    }

    // Map C++ objects to chai, load colors, modules etc.
    void initEnvironmentAndLoadScript() {

        chai = new ChaiScript();
        registerModules();
        registerGraphics();

        // Try to evaluate the script
        try {
            chai->eval(parser->getScript());
        } catch (const chaiscript::exception::eval_error &e) {
            Logger::logError("Error while evaluating script: " + std::string(e.what()));
            return;
        }

        validScript = true;
        Logger::logDebug("Script loaded successfully");
    }

    // Used only in debug, reloads the script if it has changed on disk
    void reloadScriptIfChanged() {
        if (!parser->scriptChanged()) {
            return;
        }
        delete chai;

        // Parses the script file, again
        parser->reloadScript();

        // Resets runner state by clearing drawables
        clearDrawables();

        // Reloads script environment
        initEnvironmentAndLoadScript();
    }

    // Loop
    void renderNextCanvasLayer(CanvasLayer *canvas) override {

#if DEBUG
        reloadScriptIfChanged();
#endif

        // If script is invalid don't try to render
        if(!validScript) {
            return;
        }


        // Try to evaluate the loop function
        try {
            chai->eval("loop()");
        } catch (const chaiscript::exception::eval_error &e) {
            Logger::logError("Error while evaluating loop: " + std::string(e.what()));
            validScript = false;
            return;
        }
    }


    /*
    |-----------------------------------------------------------------
    | Constructor shortcuts
    |-----------------------------------------------------------------
    */
    DrawableText *createDrawableText() {
        auto text = new DrawableText(this, parser);
        return text;
    }

    DrawableRectangle *createDrawableRectangle() {
        auto rectangle = new DrawableRectangle(this);
        return rectangle;
    }

    void registerGraphics() {

        // COLORS
        chai->add_global(chaiscript::var(WHITE_COLOR), "WHITE_COLOR");
        chai->add_global(chaiscript::var(BLACK_COLOR), "BLACK_COLOR");
        chai->add_global(chaiscript::var(RED_COLOR), "RED_COLOR");
        chai->add_global(chaiscript::var(GREEN_COLOR), "GREEN_COLOR");
        chai->add_global(chaiscript::var(BLUE_COLOR), "BLUE_COLOR");
        chai->add_global(chaiscript::var(YELLOW_COLOR), "YELLOW_COLOR");
        chai->add_global(chaiscript::var(CYAN_COLOR), "CYAN_COLOR");
        chai->add_global(chaiscript::var(MAGENTA_COLOR), "MAGENTA_COLOR");
        chai->add(fun(&RandomColor), "RANDOM_COLOR");

        // Register basic canvas functions and bind them to the canvas object
        chai->add(fun(&CanvasLayer::SetPixel, getCanvasTemplate()), "_setPixel");
        chai->add(fun(&CanvasLayer::Clear, getCanvasTemplate()), "_clear");
        chai->add(fun(&CanvasLayer::width, getCanvasTemplate()), "_canvasWidth");
        chai->add(fun(&CanvasLayer::height, getCanvasTemplate()), "_canvasHeight");

        // Register drawable functions
        chai->add(fun(&Drawable::setColor), "setColor");
        chai->add(fun(&Drawable::moveTo), "moveTo");
        chai->add(fun(&Drawable::translate), "translate");
        chai->add(fun(&Drawable::translateX), "translateX");
        chai->add(fun(&Drawable::translateY), "translateY");
        chai->add(fun(&Drawable::animateTo), "animateTo");
        chai->add(fun(&Drawable::hide), "hide");
        chai->add(fun(&Drawable::show), "show");
        chai->add(fun(&Drawable::isVisible), "isVisible");
        chai->add(fun(&Drawable::isAnimating), "isAnimating");
        chai->add(fun(&Drawable::getX), "getX");
        chai->add(fun(&Drawable::getY), "getY");

        // Register configurable drawable functions
        chai->add(chaiscript::base_class<Drawable, ConfigurableDrawable>());
        chai->add(fun(&ConfigurableDrawable::loadConfig), "loadConfig");

        // TEXT
        chai->add(chaiscript::base_class<Drawable, DrawableText>());
        chai->add(chaiscript::base_class<ConfigurableDrawable, DrawableText>());
        chai->add(user_type < DrawableText > (), "DrawableText");
        chai->add(fun(&DynamicWidget::createDrawableText, this), "_DrawableText");
        chai->add(fun(&DrawableText::setText), "setText");

        // SHAPES
        chai->add(chaiscript::base_class<Drawable, DrawableShape>());

        // RECTANGLE
        chai->add(chaiscript::base_class<Drawable, DrawableRectangle>());
        chai->add(chaiscript::base_class<DrawableShape, DrawableRectangle>());
        chai->add(user_type < DrawableRectangle > (), "DrawableRectangle");
        chai->add(fun(&DynamicWidget::createDrawableRectangle, this), "_DrawableRectangle");
        chai->add(fun(&DrawableRectangle::setSize), "setSize");

//        // PPM
//        chai->add(user_type<DrawablePPM> (), "_DrawablePPM");
//        chai->add(chaiscript::base_class<Drawable, DrawablePPM>());
    }

};

#endif