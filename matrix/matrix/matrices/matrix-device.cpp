#ifndef MATRIX_DEVICE_CPP
#define MATRIX_DEVICE_CPP

#include "../../external/rpi-rgb-led-matrix/include/canvas.h"
#include "../../external/rpi-rgb-led-matrix/include/led-matrix.h"
#include "../../logger/logger.h"

using namespace rgb_matrix;
class MatrixDevice : public Canvas {

public:

    MatrixDevice(){}

    int width() const {
        return MATRIX_COLS;
    }m
    int height() const {
        return MATRIX_ROWS;
    }

    // Canvas interface, implemented just to satisfy the compiler
    // We should never act directly on the matrix device, we should use a canvas buffer instead
    void Clear() {
        Logger::logFatal("MatrixDevice::Clear() should never be called directly, use a canvas buffer instead");
    }
    void SetPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue){
        Logger::logFatal("MatrixDevice::SetPixel() should never be called directly, use a canvas buffer instead");
    }
    void Fill(uint8_t red, uint8_t green, uint8_t blue){
        Logger::logFatal("MatrixDevice::Fill() should never be called directly, use a canvas buffer instead");
    }

    virtual Canvas* CreateFrameCanvas() = 0;
    virtual void SwapFrameCanvas(Canvas *canvas) = 0;
};



#endif