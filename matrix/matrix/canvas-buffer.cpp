#ifndef CANVAS_BUFFER_CPP
#define CANVAS_BUFFER_CPP

#include <list>
#include "matrices/matrix-device.cpp"
#include "canvas-layer.cpp"
#include "../logger/logger.h"

/// This class is responsible for buffering the canvas frames
/// This is useful when you want to render multiple runners on the matrix and be able to show them all at once
/// The buffer will be able to swap the frames on the matrix without flickering
class CanvasBuffer {

private:
    MatrixDevice *matrix;
    std::list<Canvas*> buffer;

public:
    CanvasBuffer(MatrixDevice *matrix, int bufferSize) {
        this->matrix = matrix;
        for (int i = 0; i < bufferSize; i++) {
            buffer.push_back(matrix->CreateFrameCanvas());
        }
    }

    void paintPartialLayerOnCurrentFrame(CanvasLayer *canvasLayer) {
        auto *currentFrame = buffer.front();
        canvasLayer->paintOntoCanvas(currentFrame);
    }

    void loadNextFrameOnMatrix() {
        auto *currentFrame = buffer.front();
        buffer.pop_front(); // Remove the front frame from the buffer
        matrix->SwapFrameCanvas(currentFrame); // Swap the frame on the matrix
        buffer.push_back(currentFrame); // Push it to the back

        // Prepare next frame
        auto *nextFrame = buffer.front();
        nextFrame->Clear();
    }

    ~CanvasBuffer() {

#if SIMULATION
        for (auto *frame : buffer) {
            delete frame;
        }
#endif
        buffer.clear();
    }
};

#endif
