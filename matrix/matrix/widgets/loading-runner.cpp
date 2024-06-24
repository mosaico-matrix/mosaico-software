#ifndef LOADING_RUNNER_CPP
#define LOADING_RUNNER_CPP

#include <cstdlib> // for rand() and srand()
#include <ctime>   // for time()
#include "../../external/rpi-rgb-led-matrix/include/led-matrix.h"
#include "../../logger/logger.h"
#include "matrix-widget.h"
#include <cmath> // For easing functions
#include "../drawables/drawable-shape.cpp"

class LoadingRunner : public MatrixWidget {

private:
    int squareSize = 8;
    int squaresCount = 20;

private:

public:

    void bouncingSquares(CanvasLayer *canvas) {
        for (int i = 0; i < squares.size(); i++) {
            auto square = squares[i];
            if (!square->isAnimating()) {
                //square->animateTo(rand() % (canvas->width() - squareSize), rand() % (canvas->height() - squareSize));
            }
        }
    }

    void randomSquares(CanvasLayer *canvas) {
        // Cycle on spots where a potential square could be drawn
        for (int i = 0; i < canvas->width(); i += squareSize) {
            for (int j = 0; j < canvas->height(); j += squareSize) {

                // Throw a coin to decide if we draw the square
                if (rand() % 2 == 0) {
                    continue;
                }

                // Draw the square
                Color color = RandomColor();
                for (int x = i; x < i + squareSize; x++) {
                    for (int y = j; y < j + squareSize; y++) {
                        canvas->SetPixel(x, y, color.r, color.g, color.b);
                    }
                }
            }
        }
    }

    void renderSquareForeachColor(CanvasLayer *canvas) {
        canvas->Clear();
        int startX = 0;
        int startY = 0;
        for (int i = 0; i < COLORS.size(); i++) {
            // Draw a square for each color
            Color color = COLORS[i];
            for (int x = startX; x < startX + squareSize; x++) {
                for (int y = startY; y < startY + squareSize; y++) {
                    canvas->SetPixel(x, y, color.r, color.g, color.b);
                }
            }

            startX += squareSize;
            if (startX >= canvas->width()) {
                startX = 0;
                startY += squareSize;
            }
        }
    }


    std::vector<DrawableRectangle *> squares;

    LoadingRunner(CanvasLayerPosition position) : MatrixWidget(2, position) {

        // Initialize random seed
        srand(time(NULL));

//        // Create squares
//        for (int i = 0; i < squaresCount; i++) {
//            auto square = new DrawableRectangle(squareSize, squareSize);
//            square->setColor(RandomColor());
//            square->moveTo(rand() % 64, rand() % 64);
//            registerDrawable(square);
//            squares.push_back(square);
//        }
    }

    void renderNextCanvasLayer(CanvasLayer *canvas) override {
        //bouncingSquares(canvas);
        randomSquares(canvas);
        //renderSquareForeachColor(canvas);
    }

};

#endif //LOADING_RUNNER_CPP
