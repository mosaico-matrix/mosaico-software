#ifndef CANVAS_LAYER_CPP
#define CANVAS_LAYER_CPP

#include "../logger/logger.h"
#include <list>
#include "../external/rpi-rgb-led-matrix/include/led-matrix.h"

using namespace rgb_matrix;

enum CanvasLayerPosition {
    UNSET = -1,
    FULL = 0,
    TOP = 1,
    TOP_LEFT = 2,
    TOP_RIGHT = 3,
    BOTTOM = 4,
    BOTTOM_LEFT = 5,
    BOTTOM_RIGHT = 6,
    LEFT = 7,
    RIGHT = 8
};

/// A canvas layer is basically a layer that can be painted on by a runner
/// It is used to create a composite image that can be displayed on the matrix
/// Note that the canvas layer can be narrower and/or shorter than the matrix
class CanvasLayer : public Canvas {

private:

    struct Point {
        int x;
        int y;
    };
    struct Pixel {
        Point point;
        uint8_t red;
        uint8_t green;
        uint8_t blue;
    };

    int _width;                     // The width of the layer, this can be smaller due to padding
    int _height;                    // The height of the layer, this can be smaller due to padding
    int originalWidth;              // The width of the layer without padding
    int originalHeight;             // The height of the layer without padding
    std::list<Pixel> pixels;        // The pixels to be painted on the final canvas
    Point positionInMatrix;               // The start point of the layer in relation to the matrix
    bool bordered = false;          // If the layer should have a border
    Color borderColor = Color(255, 255, 255); // The color of the border
    int padding = 0;                    // The padding of the layer


    void _setPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue, bool ignoreBounds = false)
    {
        //Ignore pixels outside the layer
        if (x < 0 || x > _width-1 || y < 0 || y > _height-1) {
            if (!ignoreBounds)
            return;
        }

        int boundedX = x + positionInMatrix.x + padding + (bordered ? 1 : 0);
        int boundedY = y + positionInMatrix.y + padding + (bordered ? 1 : 0);

        Point point = {ignoreBounds ? x : boundedX, ignoreBounds ? y : boundedY};
        Pixel pixel = {point, red, green, blue};
        pixels.push_back(pixel);
    }


    void updateCanvasDimensions() {
        _width = originalWidth - padding*2 - (bordered ? 2 : 0);
        _height = originalHeight - padding*2 - (bordered ? 2 : 0);
    }

    // Restore the canvas dimensions to the original dimensions
    void restoreCanvasDimensions() {
        _width = originalWidth;
        _height = originalHeight;
    }

    // Draw a border around the layer
    void createBorder() {

        // Start drawing from original start point
        int x = positionInMatrix.x;
        int y = positionInMatrix.y;

        // Draw top border
        for (int i = 0; i < originalWidth; i++) {
            _setPixel(x + i, y, borderColor.r, borderColor.g, borderColor.b, true);
        }

        // Draw bottom border
        for (int i = 0; i < originalWidth; i++) {
            _setPixel(x + i, y + originalHeight - 1, borderColor.r, borderColor.g, borderColor.b, true);
        }

        // Draw left border
        for (int i = 0; i < originalHeight; i++) {
            _setPixel(x, y + i, borderColor.r, borderColor.g, borderColor.b, true);
        }

        // Draw right border
        for (int i = 0; i < originalHeight; i++) {
            _setPixel(x + originalWidth - 1, y + i, borderColor.r, borderColor.g, borderColor.b, true);
        }
    }


public:

    CanvasLayer(CanvasLayerPosition position)  {


        // Set the layer position and start point
        switch (position) {
            case FULL:
                _width = MATRIX_COLS;
                _height = MATRIX_ROWS;
                positionInMatrix = {0, 0};
                break;
            case TOP:
                _width = MATRIX_COLS;
                _height = MATRIX_ROWS / 2;
                positionInMatrix = {0, 0};
                break;
            case TOP_LEFT:
                _width = MATRIX_COLS / 2;
                _height = MATRIX_ROWS / 2;
                positionInMatrix = {0, 0};
                break;
            case TOP_RIGHT:
                _width = MATRIX_COLS / 2;
                _height = MATRIX_ROWS / 2;
                positionInMatrix = {MATRIX_COLS / 2, 0};
                break;
            case BOTTOM:
                _width = MATRIX_COLS;
                _height = MATRIX_ROWS / 2;
                positionInMatrix = {0, MATRIX_ROWS / 2};
                break;
            case BOTTOM_LEFT:
                _width = MATRIX_COLS / 2;
                _height = MATRIX_ROWS / 2;
                positionInMatrix = {0, MATRIX_ROWS / 2};
                break;
            case BOTTOM_RIGHT:
                _width = MATRIX_COLS / 2;
                _height = MATRIX_ROWS / 2;
                positionInMatrix = {MATRIX_COLS / 2, MATRIX_ROWS / 2};
                break;
            case LEFT:
                _width = MATRIX_COLS / 2;
                _height = MATRIX_ROWS;
                positionInMatrix = {0, 0};
                break;
            case RIGHT:
                _width = MATRIX_COLS / 2;
                _height = MATRIX_ROWS;
                positionInMatrix = {MATRIX_COLS / 2, 0};
                break;
            default:
                _width = 32;
                _height = 64;
                positionInMatrix = {5, 0};
                break;
        }
        originalWidth = _width;
        originalHeight = _height;
    }

    ~CanvasLayer() {
        pixels.clear();
    }


    int width() const {
        return _width;
    }

    int height() const {
        return _height;
    }

    void SetPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue) {
        _setPixel(x, y, red, green, blue);
    }

    // Merge to actual canvas
    void paintOntoCanvas(Canvas *canvas) {

        // Draw border if needed
        if (bordered)
        {
            createBorder();
        }

        // Draw pixels on the final canvas
        for (auto pixel: pixels) {
            int x = pixel.point.x;
            int y = pixel.point.y;
            canvas->SetPixel(x, y, pixel.red, pixel.green, pixel.blue);
        }
    }

    // Fill screen with given 24bpp color.
    void Fill(uint8_t red, uint8_t green, uint8_t blue) {

        // If the color is black, clear the pixels for optimization
        if(red == 0 && green == 0 && blue == 0) {
            pixels.clear();
            return;
        }

        for (int x = 0; x < _width; x++) {
            for (int y = 0; y < _height; y++) {
                SetPixel(x, y, red, green, blue);
            }
        }
    }

    // Clear screen to be all black.
    void Clear() {
        pixels.clear();
    }


    void setBorder(Color color) {
        bordered = true;
        borderColor = color;
        updateCanvasDimensions();
    }

    void setPadding(int padding) {
        this->padding = padding;
        updateCanvasDimensions();
    }

};


#endif