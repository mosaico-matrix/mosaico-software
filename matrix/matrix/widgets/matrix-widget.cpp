#pragma once
#include <memory>
#include "matrix-widget.h"
#include <algorithm> // Ensure this is included for std::remove

MatrixWidget::MatrixWidget() {
    Logger::logDebug("Matrix widget created");
}

void MatrixWidget::incrementFrameIndex() {
    renderedFrameCount++;
    if (renderedFrameCount > UINT_MAX) {
        renderedFrameCount = 0;
    }
}

CanvasLayer* MatrixWidget::renderCanvasLayer() {

    Logger::logDebug("Rendering canvas layer");

    // Check if lastRenderedFrame has been initialized
    if (lastRenderedFrame == nullptr) {
        lastRenderedFrame = new CanvasLayer(runnerPosition);
    }

    // If framesPerSecond is 0, we should only render the first frame
    if (framesPerSecond == 0 && !firstRender) {
        return lastRenderedFrame;
    }
    if (firstRender) {
        this->initialize();
        firstRender = false;
    }

    lastRenderedFrame->Clear();
    for (Drawable* drawable : registeredDrawables) {
        Logger::logDebug("Drawing drawable");
        drawable->draw(lastRenderedFrame);
    }

    renderNextCanvasLayer(lastRenderedFrame);
    incrementFrameIndex();
    return lastRenderedFrame;
}

void MatrixWidget::resetRenderedFrameCount() {
    renderedFrameCount = 0;
}

void MatrixWidget::registerDrawable(Drawable* drawable) {
    Logger::logDebug("Registering drawable");
    drawable->setFrameDuration(1000 / framesPerSecond);
    registeredDrawables.push_back(drawable);
}

void MatrixWidget::clearDrawables() {
    registeredDrawables.clear();
}

void MatrixWidget::unregisterDrawable(Drawable* drawable) {
    registeredDrawables.erase(
            std::remove(registeredDrawables.begin(), registeredDrawables.end(), drawable),
            registeredDrawables.end()
    );
}
