#pragma once

#include <cstdlib> // for rand() and srand()
#include <ctime>   // for time()
#include "../../external/rpi-rgb-led-matrix/include/led-matrix.h"
#include "../../logger/logger.h"
#include "matrix-widget.h"
#include <cmath> // For easing functions
#include "../drawables/drawable-shape.cpp"

class IdleWidget : public MatrixWidget {


public:

    IdleWidget() : MatrixWidget() {

        // Configure
        setFps(1);
        setCanvasPosition(CanvasLayerPosition::FULL);
    }

    void renderNextCanvasLayer(CanvasLayer *canvas) override {
        // pass
    }

    void initialize() override {}

};

