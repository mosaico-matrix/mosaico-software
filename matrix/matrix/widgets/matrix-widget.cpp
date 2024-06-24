#pragma once
#include <memory>
#include "matrix-widget.h"


MatrixWidget::MatrixWidget(int framesPerSecond, CanvasLayerPosition position)
        : framesPerSecond(framesPerSecond), runnerPosition(position) {
    if (framesPerSecond > MATRIX_REFRESH_RATE) {
        Logger::logFatal("Requested frames per second is higher than the matrix refresh rate");
    }

    Logger::logDebug("Matrix widget created");
    lastRenderedFrame = new CanvasLayer(runnerPosition);
}

void MatrixWidget::incrementFrameIndex() {
    renderedFrameCount++;
    if (renderedFrameCount > UINT_MAX) {
        renderedFrameCount = 0;
    }
}

CanvasLayer* MatrixWidget::renderCanvasLayer() {

    // If framesPerSecond is 0, we should only render the first frame
    if (framesPerSecond == 0 && !firstRender) {
        return lastRenderedFrame;
    }
    if (firstRender) {
        firstRender = false;
    }

    lastRenderedFrame->Clear();
    for (Drawable* drawable : registeredDrawables) {
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
    drawable->setFrameDuration(1000 / framesPerSecond);
    registeredDrawables.push_back(drawable);
}

void MatrixWidget::clearDrawables() {
    registeredDrawables.clear();
}
