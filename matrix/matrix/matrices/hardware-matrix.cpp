#ifndef HARDWARE_MATRIX_CPP
#define HARDWARE_MATRIX_CPP

#include "matrix-device.cpp"

class HardwareMatrix : public MatrixDevice {

private:
    RGBMatrix *matrix;

public:
    HardwareMatrix() {

        // Configure the RGBMatrix
        RGBMatrix::Options matrix_options;
        rgb_matrix::RuntimeOptions runtime_opt;
        matrix_options.rows = MATRIX_ROWS;
        matrix_options.chain_length = 1;
        matrix_options.cols = MATRIX_COLS;
        matrix_options.hardware_mapping = "adafruit-hat";
        matrix_options.brightness = 100; // TODO: Change this to 100 maybe
        //matrix_options.limit_refresh_rate_hz = 80;
        runtime_opt.gpio_slowdown = 5; // Adjust this for flickering

        // Create the RGBMatrix object
        RGBMatrix *matrix = RGBMatrix::CreateFromOptions(matrix_options, runtime_opt);
        if (matrix == NULL) {
            Logger::logFatal("Error initializing RGBMatrix");
        }
        this->matrix = matrix;
    }

    Canvas* CreateFrameCanvas()
    {
        return matrix->CreateFrameCanvas();
    }

    void SwapFrameCanvas(Canvas *canvas){
        matrix->SwapOnVSync((FrameCanvas*)canvas);
    }
};

#endif