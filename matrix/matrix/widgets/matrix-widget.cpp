#include "matrix-widget.h"
#include <limits>
#include <algorithm>

MatrixWidget::MatrixWidget() {
    Logger::logDebug("Matrix widget created");
}

void MatrixWidget::incrementFrameIndex() {
    renderedFrameCount++;
    if (renderedFrameCount > std::numeric_limits<unsigned int>::max()) {
        renderedFrameCount = 0;
    }
}

CanvasLayer* MatrixWidget::renderCanvasLayer() {
    if (lastRenderedFrame == nullptr) {
        lastRenderedFrame = new CanvasLayer(runnerPosition);
    }

    if (framesPerSecond == 0 && !firstRender) {
        return lastRenderedFrame;
    }
    if (firstRender) {
        firstRender = false;
    }

    lastRenderedFrame->Clear();
    for (const auto& drawable : registeredDrawables) {
        drawable->draw(lastRenderedFrame);
    }

    renderNextCanvasLayer(lastRenderedFrame);
    incrementFrameIndex();
    return lastRenderedFrame;
}


void MatrixWidget::registerDrawable(std::unique_ptr<Drawable> drawable) {
    drawable->setFrameDuration(1000 / framesPerSecond);
    registeredDrawables.push_back(std::move(drawable));
}

void MatrixWidget::clearDrawables() {
    registeredDrawables.clear();
}

void MatrixWidget::unregisterDrawable(Drawable* drawable) {
    auto it = std::remove_if(registeredDrawables.begin(), registeredDrawables.end(),
                             [drawable](const std::unique_ptr<Drawable>& ptr) {
                                 return ptr.get() == drawable;
                             });
    if (it != registeredDrawables.end()) {
        registeredDrawables.erase(it, registeredDrawables.end()); // This will delete the matching Drawable object
    }
}
