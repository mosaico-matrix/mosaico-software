#ifndef TEST_WIDGET_CPP
#define TEST_WIDGET_CPP

#include "../../external/rpi-rgb-led-matrix/include/led-matrix.h"
#include "../../logger/logger.h"
#include "matrix-widget.h"

class TestWidget : public MatrixWidget {


private:

public:

    TestWidget() : MatrixWidget() {

        // Configure
        setFps(20);
        setCanvasPosition(CanvasLayerPosition::FULL);
    }

    void renderNextCanvasLayer(CanvasLayer *canvas) override {
        auto square = createRectangle();
        square->setSize(10, 10);
        square->translate(20,20);
    }

    ~TestWidget() override {
        Logger::logDebug("LoadingWidget destroyed");
    }

};



#endif