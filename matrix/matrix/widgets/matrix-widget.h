#ifndef MATRIX_RUNNER_H
#define MATRIX_RUNNER_H

#include <climits>
#include <vector>

#include "../../external/rpi-rgb-led-matrix/include/led-matrix.h"
#include "../../logger/logger.h"
#include "../drawables/drawable.h"
#include "../canvas-layer.cpp"
#include "../drawables/drawable-text.cpp"
#include "../drawables/drawable-shape.cpp"
#include "../drawables/drawable-ppm.cpp"

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

    /// Sets the frame rate of the widget
    void setFps(unsigned int fps) { framesPerSecond = fps; }

    /// Sets the position of the canvas on the matrix
    void setCanvasPosition(CanvasLayerPosition position) { runnerPosition = position; }

    /// This does some checks before calling the renderNextCanvasLayer method in the child class
    CanvasLayer* renderCanvasLayer();

    /// Returns the widget's frame rate
    [[nodiscard]] unsigned int getFrameRate() const { return framesPerSecond; }

    /// Returns the number of frames rendered (this is reset to prevent overflow)
    [[nodiscard]] unsigned int getRenderedFrameCount() const { return renderedFrameCount; }

    /// Resets the frame count
    void resetRenderedFrameCount();

    /// Add a new drawable to the widget drawable list
    void registerDrawable(Drawable* drawable);

    /// Remove a drawable from the widget drawable list
    void unregisterDrawable(Drawable* drawable);

    /// Clear all drawables
    void clearDrawables();

    /// Returns the last rendered frame
    [[nodiscard]] CanvasLayer* getLastRenderedFrame() const { return lastRenderedFrame; }

    /// This basically returns the last rendered frame, but has a different name because can serve a different purpose
    [[nodiscard]] CanvasLayer* getCanvasTemplate() const { return lastRenderedFrame; }

    /// Virtual destructor
    virtual ~MatrixWidget() = default;

    /*
    * Add drawables shortcuts
    */
    DrawableText *createText(){
        auto *drawable = new DrawableText();
        registerDrawable(drawable);
        return drawable;
    }
    DrawableRectangle *createRectangle(){
        auto *drawable = new DrawableRectangle();
        registerDrawable(drawable);
        return drawable;
    }

    DrawablePPM *createPPM(){
        auto *drawable = new DrawablePPM();
        registerDrawable(drawable);
        return drawable;
    }

protected:
    CanvasLayerPosition runnerPosition = CanvasLayerPosition::FULL;
    MatrixWidget();

    // Pure virtual method to be overridden for rendering
    virtual void renderNextCanvasLayer(CanvasLayer* canvas) = 0;
};

#endif
