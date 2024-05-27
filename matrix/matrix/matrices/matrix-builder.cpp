#ifndef MATRIX_BUILDER_CPP
#define MATRIX_BUILDER_CPP

#include "../../external/rpi-rgb-led-matrix/include/canvas.h"
#include "../../external/rpi-rgb-led-matrix/include/led-matrix.h"

#include "../../logger/logger.h"
#include "hardware-matrix.cpp"

#if SIMULATION
#include "x11-matrix.cpp"
#endif

class MatrixBuilder
{
public:
    static MatrixDevice* build()
    {
#if SIMULATION
        return new x11matrix::X11Matrix();
#else
        return new HardwareMatrix();
#endif
    }
};


#endif

//
//#ifndef MATRIX_PROXY_CPP
//#define MATRIX_PROXY_CPP
//
//
//#include "../../external/rpi-rgb-led-matrix/include/canvas.h"
//#include "../../external/rpi-rgb-led-matrix/include/led-matrix.h"
//#include "x11-matrix.cpp"
//#include "../../logger/logger.h"
//
//using namespace rgb_matrix;
//
//class MatrixProxy : public Canvas {
//
//public:
//
//#ifdef DEBUG
//
//    mock_matrix::MockMatrix *matrix;
//    MatrixProxy()
//    {
//        matrix = new mock_matrix::MockMatrix();
//    }
//     mock_matrix::MockFrameCanvas* CreateFrameCanvas() {
//        return matrix->CreateFrameCanvas();
//    }
//
//    void swapFrameCanvas(mock_matrix::MockFrameCanvas *canvas) {
//        matrix->Draw(canvas);
//    }
//#else
//    RGBMatrix *matrix;
//
//    MatrixProxy() {
//
//        RGBMatrix::Options matrix_options;
//        RuntimeOptions runtime_opt;
//        // These are the defaults when no command-line flags are given, for now they are hard-coded
//        matrix_options.rows = MATRIX_ROWS;
//        matrix_options.chain_length = 1;
//        matrix_options.cols = MATRIX_COLS;
//        matrix_options.hardware_mapping = "adafruit-hat";
//        matrix_options.brightness = 80; // TODO: Change this to 100 maybe
//        //matrix_options.limit_refresh_rate_hz = 80;
//        runtime_opt.gpio_slowdown = 5; // Adjust this for flickering
//
//        // Create the RGBMatrix object
//        RGBMatrix *matrix = RGBMatrix::CreateFromOptions(matrix_options, runtime_opt);
//        if (matrix == NULL) {
//            Logger::logFatal("Error initializing RGBMatrix");
//        }
//        this->matrix = matrix;
//    }
//
//    FrameCanvas *CreateFrameCanvas() {
//        return matrix->CreateFrameCanvas();
//    }
//
//    void swapFrameCanvas(FrameCanvas *canvas) {
//        matrix->SwapOnVSync(canvas);
//    }
//
//#endif
//
//
//    void Clear() {
//        matrix->Clear();
//    }
//
//    int width() const {
//        return MATRIX_COLS;
//    }
//
//    int height() const {
//        return MATRIX_ROWS;
//    }
//
//    ~MatrixProxy() {}
//
//    void SetPixel(int x, int y,
//                  uint8_t red, uint8_t green, uint8_t blue) {
//        matrix->SetPixel(x, y, red, green, blue);
//    }
//
//    void Fill(uint8_t red, uint8_t green, uint8_t blue) {
//        matrix->Fill(red, green, blue);
//    }
//};
//
//#endif //MATRIX_PROXY_CPP