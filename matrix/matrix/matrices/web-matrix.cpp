#pragma once

#include "matrix-device.cpp"
#include "../../external/rpi-rgb-led-matrix/include/led-matrix.h"

using namespace rgb_matrix;

// I can make this matrix to create a stream of images served through a RTMP server and ffmpeg
class WebCanvas : public Canvas {

public:

    WebCanvas()
    {

    }

    ~WebCanvas() {
    }


    void SetPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue) {
    }

    void Fill(uint8_t red, uint8_t green, uint8_t blue) {
    }

    void Clear() {
    }

    int width() const {
        return MATRIX_ROWS;
    }

    int height() const {
        return MATRIX_COLS;
    }

};

class WebMatrix : public MatrixDevice {
public:
    WebMatrix() {
    }

    Canvas *CreateFrameCanvas() {
        return new WebCanvas();
    }

    void SwapFrameCanvas(Canvas *canvas) {
        // Do nothing
    }

};
