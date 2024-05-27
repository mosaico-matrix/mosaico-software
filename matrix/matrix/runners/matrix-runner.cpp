#pragma once
#include <memory>
#include "matrix-runner.h"


MatrixRunner::MatrixRunner(int framesPerSecond, CanvasLayerPosition position)
        : framesPerSecond(framesPerSecond), runnerPosition(position) {
    if (framesPerSecond > MATRIX_REFRESH_RATE) {
        Logger::logFatal("Requested frames per second is higher than the matrix refresh rate");
    }

    Logger::logDebug("MatrixRunner created");
    lastRenderedFrame = new CanvasLayer(runnerPosition);
}

void MatrixRunner::incrementFrameIndex() {
    renderedFrameCount++;
    if (renderedFrameCount > UINT_MAX) {
        renderedFrameCount = 0;
    }
}

CanvasLayer* MatrixRunner::renderCanvasLayer() {

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

void MatrixRunner::resetRenderedFrameCount() {
    renderedFrameCount = 0;
}

void MatrixRunner::registerDrawable(Drawable* drawable) {
    drawable->setFrameDuration(1000 / framesPerSecond);
    registeredDrawables.push_back(drawable);
}



void MatrixRunner::clearDrawables() {
    registeredDrawables.clear();
}
