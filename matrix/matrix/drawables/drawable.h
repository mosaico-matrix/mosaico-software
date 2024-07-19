#pragma once

#include "../../external/rpi-rgb-led-matrix/include/graphics.h"
#include "../../logger/logger.h"
#include "../../configs.cpp"
#include <chrono>
#include <cmath>
#include "colors.cpp"
#include <string>

// Class representing a drawable object on the matrix
class Drawable {
protected:
    // Positioning
    int xPosition = 0, yPosition = 0;               // Current position
    float xPositionFloat = 0, yPositionFloat = 0;   // Current position with subpixel precision

    // Color of the drawable
    rgb_matrix::Color color = WHITE_COLOR;

    // Visibility state
    bool visible = true;

    // Animation parameters
    int targetX = 0, targetY = 0;                                   // Target position for animation
    float stepX = 0, stepY = 0;                                     // Steps for each frame of animation
    rgb_matrix::Color targetColor = WHITE_COLOR;
    bool colorAnimating = false;                                    // If color is being animated
    bool animating = false;                                         // If position is being animated
    unsigned int frameDurationMs;                                   // Duration of each frame in milliseconds
    unsigned int currentAnimationFramesLeft = 0;                    // Frames left in the current animation

    // Abstract method to draw the drawable on the canvas, this must be implemented by subclasses
    virtual void _draw(rgb_matrix::Canvas *canvas) = 0;

public:
    struct Pixel {
        Pixel(int r = 0, int g = 0, int b = 0) : red(r), green(g), blue(b) {}

        uint8_t red;
        uint8_t green;
        uint8_t blue;
    };

    Drawable();
    virtual ~Drawable();

    // Set the duration of each animation frame
    void setFrameDuration(unsigned int frameDurationMs);

    // Directly set the position of the drawable
    void moveTo(int x, int y);

    // Offset the position of the drawable by dx and dy
    void translateBy(int dx, int dy);

    // Offset the position of the drawable by dx
    void translateXBy(int dx);

    // Offset the position of the drawable by dy
    void translateYBy(int dy);

    // Set the color of the drawable
    void setColor(rgb_matrix::Color color);

    // Set the color of the drawable using hex color
    void setHexColor(std::string hexColor);

    // Animate the drawable to a new position over a specified duration
    void animateTo(int x, int y, unsigned int animationDurationMs);

    // Hide the drawable from the canvas
    void hide();

    // Show the drawable on the canvas
    void show();

    // Check if the drawable is visible
    bool isVisible() const;

    // Check if the drawable is animating
    bool isAnimating() const;

    // Get the current X position of the drawable
    int getX() const;

    // Get the current Y position of the drawable
    int getY() const;

    // Draw the drawable on the canvas
    void draw(rgb_matrix::Canvas *canvas);
};