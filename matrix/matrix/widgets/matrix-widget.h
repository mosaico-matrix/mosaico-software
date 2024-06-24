#ifndef MATRIX_RUNNER_H
#define MATRIX_RUNNER_H

#include <climits>
#include <vector>

#include "../../external/rpi-rgb-led-matrix/include/led-matrix.h"
#include "../../logger/logger.h"
#include "../drawables/drawable.h"
#include "../canvas-layer.cpp"

// Available games (add more as needed)
enum class MatrixGameEnum {
    PIXEL_MOVER = -1,
    TETRIS = 0,
    // Add more games here
};

class CanvasLayer;
class Drawable;

class MatrixWidget {
private:
    unsigned int renderedFrameCount = 0;            // Current frame index
    unsigned int framesPerSecond = 0;               // FPS
    unsigned long runningTime = 0;                  // Time runner has been running
    bool firstRender = true;                        // Flag for first render
    CanvasLayer* lastRenderedFrame = nullptr;       // Last rendered frame
    std::vector<Drawable*> registeredDrawables;     // Drawables to be rendered

    void incrementFrameIndex();

public:
    CanvasLayer* renderCanvasLayer();
    int getFrameRate() const { return framesPerSecond; }
    int getRenderedFrameCount() const { return renderedFrameCount; }
    void resetRenderedFrameCount();
    void registerDrawable(Drawable* drawable);
    ~MatrixWidget() = default;
    void clearDrawables();
    CanvasLayer* getCanvasTemplate() const { return lastRenderedFrame; }
    CanvasLayer* getLastRenderedFrame() const { return lastRenderedFrame; }

protected:
    CanvasLayerPosition runnerPosition;

    // Constructor with FPS and position
    MatrixWidget(int framesPerSecond = 30, CanvasLayerPosition position = FULL);

    // Pure virtual method to be overridden for rendering
    virtual void renderNextCanvasLayer(CanvasLayer* canvas) = 0;
};

#endif
