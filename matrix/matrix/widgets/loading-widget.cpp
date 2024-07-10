#ifndef LOADING_RUNNER_CPP
#define LOADING_RUNNER_CPP

#include <cstdlib> // for rand() and srand()
#include <ctime>   // for time()
#include "../../external/rpi-rgb-led-matrix/include/led-matrix.h"
#include "../../logger/logger.h"
#include "matrix-widget.h"
#include <cmath> // For easing functions
#include "../drawables/drawable-shape.cpp"

class LoadingWidget : public MatrixWidget {

private:
    int squareSize = 4;
    int squaresCount = 20;

private:

public:

    void randomSquares(CanvasLayer *canvas) {
        // Cycle on spots where a potential square could be drawn
        for (int i = 0; i < canvas->width(); i += squareSize) {
            for (int j = 0; j < canvas->height(); j += squareSize) {

                // Throw a coin to decide if we draw the square
                if (rand() % 2 == 0) {
                    continue;
                }

                // Draw the square
                auto color = RandomColor();
                for (int x = i; x < i + squareSize; x++) {
                    for (int y = j; y < j + squareSize; y++) {
                        canvas->SetPixel(x, y, color);
                    }
                }
            }
        }
    }


    LoadingWidget() : MatrixWidget() {

        // Configure
        setFps(2);
        setCanvasPosition(CanvasLayerPosition::FULL);

        // Initialize random seed
        srand(time(NULL));
    }

    void renderNextCanvasLayer(CanvasLayer *canvas) override {
        randomSquares(canvas);
    }

};

#endif //LOADING_RUNNER_CPP
