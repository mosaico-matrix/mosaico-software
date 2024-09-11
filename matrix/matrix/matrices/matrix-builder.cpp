#ifndef MATRIX_BUILDER_CPP
#define MATRIX_BUILDER_CPP

#include "../../external/rpi-rgb-led-matrix/include/canvas.h"
#include "../../external/rpi-rgb-led-matrix/include/led-matrix.h"

#include "../../logger/logger.h"
#include "hardware-matrix.cpp"
#if SIMULATION
#include "x11-matrix.cpp"
#endif
#if WEB
#include "matrix-stream.cpp"
#endif

class MatrixBuilder
{
public:
    static MatrixDevice* build()
    {
#if SIMULATION
        return new x11matrix::X11Matrix();
#elif WEB
        return new MatrixStream();
#else
        return new HardwareMatrix();
#endif
    }
};


#endif