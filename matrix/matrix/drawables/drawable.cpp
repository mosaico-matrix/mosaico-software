#include "drawable.h"
#include "../widgets/matrix-widget.h"

Drawable::Drawable() {
}

Drawable::~Drawable() {}

void Drawable::setFrameDuration(unsigned int frameDurationMs) {
    this->frameDurationMs = frameDurationMs;
}

void Drawable::moveTo(int x, int y) {
    this->xPosition = x;
    this->yPosition = y;
    this->xPositionFloat = x;
    this->yPositionFloat = y;
}

void Drawable::translateX(int x) {
    this->xPosition += x;
    this->xPositionFloat += x;
}

void Drawable::translateY(int y) {
    this->yPosition += y;
    this->yPositionFloat += y;
}

void Drawable::translate(int x, int y) {
    translateX(x);
    translateY(y);
}

void Drawable::setColor(Color color) {
this->color = color;
}

void Drawable::setHexColor(std::string hexColor) {
    // Convert hex to individual RGB values
    int r, g, b;
    sscanf(hexColor.c_str(), "#%02x%02x%02x", &r, &g, &b);
    this->color = Color(r, g, b);
}

void Drawable::animateTo(int x, int y, unsigned int animationDurationMs) {
    // Set the target position and animation flag
    targetX = x;
    targetY = y;
    animating = true;

    // Calculate the distance to move in each direction
    int deltaX = targetX - xPosition;
    int deltaY = targetY - yPosition;

    // Calculate the total number of frames based on frame duration and animation duration
    currentAnimationFramesLeft = animationDurationMs / frameDurationMs;

    // Ensure at least one frame
    currentAnimationFramesLeft = std::max(currentAnimationFramesLeft, 1u);

    // Calculate the incremental change for each frame using floating-point division
    stepX = static_cast<float>(deltaX) / currentAnimationFramesLeft;
    stepY = static_cast<float>(deltaY) / currentAnimationFramesLeft;

    // Adjust stepX and stepY to ensure correct movement
    if (stepX == 0 && deltaX != 0) {
        stepX = (deltaX > 0) ? 1 : -1;
    }
    if (stepY == 0 && deltaY != 0) {
        stepY = (deltaY > 0) ? 1 : -1;
    }
}

void Drawable::hide() {
    this->visible = false;
}

void Drawable::show() {
    this->visible = true;
}

bool Drawable::isVisible() {
    return this->visible;
}

bool Drawable::isAnimating() {
    return this->animating;
}

int Drawable::getX() {
    return this->xPosition;
}

int Drawable::getY() {
    return this->yPosition;
}

void Drawable::draw(Canvas *canvas) {

    if (canvas == nullptr) {
        Logger::logFatal("Trying to draw a drawable without a canvas");
    }
    if (!visible) {
        return;
    }

    // Update the position if it's animating
    if (animating) {

        // Move the drawable
        xPositionFloat += stepX;
        yPositionFloat += stepY;

        // Round the position to the nearest integer
        xPosition = static_cast<int>(std::round(xPositionFloat));
        yPosition = static_cast<int>(std::round(yPositionFloat));

        // Check if we reached the target
        if (--currentAnimationFramesLeft == 0) {
            animating = false;
            xPosition = targetX;
            yPosition = targetY;
        }
    }
    _draw(canvas);
}
