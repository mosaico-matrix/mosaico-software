#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "../../external/rpi-rgb-led-matrix/include/graphics.h"
#include "../widgets/dynamic/dynamic-widget-parser.cpp"
#include "../../logger/logger.h"
#include "../../configs.cpp"
#include <chrono>
#include <cmath>
#include "colors.cpp"
#include <string>

using namespace std;
using namespace rgb_matrix;
class MatrixWidget;

class Drawable {
protected:
    // Positioning
    int xPosition = 0, yPosition = 0;               // Where the shape is at the moment
    float xPositionFloat = 0, yPositionFloat = 0;   // Where the shape is at the moment (including half pixels)

    // The current color of the drawable
    Color color = WHITE_COLOR;

    // Whether the drawable is visible or not
    bool visible = true;

    // Animation stuff
    int targetX = 0, targetY = 0;                                   // Where the shape is animating to
    float stepX = 0, stepY = 0;                                     // How much to move in each frame
    Color targetColor = WHITE_COLOR;
    bool colorAnimating = false;                                    // The color we are animating to
    bool animating = false;                                         // If the drawable is animating
    unsigned int frameDurationMs;                                   // Duration of each frame in milliseconds
    unsigned int currentAnimationFramesLeft = 0;                    // How many frames are left in the current animation

    virtual void _draw(Canvas *canvas) = 0;

public:

    struct Pixel {
        Pixel() : red(0), green(0), blue(0) {}

        uint8_t red;
        uint8_t green;
        uint8_t blue;
    };

    Drawable(MatrixWidget *runner);
    virtual ~Drawable();

    void setFrameDuration(unsigned int frameDurationMs);

    // Directly set the position of the drawable
    void moveTo(int x, int y);

    // Offset the position of the drawable
    void translate(int x, int y);
    void translateX(int x);
    void translateY(int y);

    void setColor(Color color);
    void animateTo(int x, int y, unsigned int animationDurationMs);
    void hide();
    void show();
    bool isVisible();
    bool isAnimating();
    int getX();
    int getY();
    void draw(Canvas *canvas);
};

class ConfigurableDrawable : public Drawable {

protected:
    DynamicWidgetParser *parser = NULL;

public:
    ConfigurableDrawable(MatrixWidget *runner, DynamicWidgetParser *parser = NULL) : Drawable(runner), parser(parser) {}
    virtual ~ConfigurableDrawable() {}
    virtual void loadConfig(string key) = 0;
};


#endif // DRAWABLE_H