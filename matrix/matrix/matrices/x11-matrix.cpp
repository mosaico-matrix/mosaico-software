#ifndef X11_MATRIX_CPP
#define X11_MATRIX_CPP

#include <cstdio>
#include <cstring>
#include "../../external/rpi-rgb-led-matrix/include/led-matrix.h"
#include <cstdint>
#include "../../logger/logger.h"
#include "../../external/rpi-rgb-led-matrix/include/canvas.h"
#include "matrix-device.cpp"

// Necessary namespace due to conflict with font.h in matrix library
namespace x11matrix {

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#define PIXEL_SIZE 13
#define MATRIX_WIDTH (MATRIX_COLS * PIXEL_SIZE)
#define MATRIX_HEIGHT (MATRIX_ROWS * PIXEL_SIZE)


    class MockFrameCanvas : public rgb_matrix::Canvas {
    private:
        Display *display;
        Window window;

    public:
        Pixmap buffer;

        MockFrameCanvas(Display *display, Visual *visual, int depth, Window window) : display(display), window(window),
                                                                                      buffer(None) {
            buffer = XCreatePixmap(display, window, MATRIX_WIDTH, MATRIX_HEIGHT, depth);
        }

        ~MockFrameCanvas() {

            // Destroy the buffer pixmap if it's created
            if (buffer != None) {
                XFreePixmap(display, buffer);
            }
        }


        void SetPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue) {
            // Draw the pixel
            XSetForeground(display, DefaultGC(display, 0), red << 16 | green << 8 | blue);
            XFillRectangle(display, buffer, DefaultGC(display, 0), x * PIXEL_SIZE, y * PIXEL_SIZE, PIXEL_SIZE,
                           PIXEL_SIZE);
        }

        void Fill(uint8_t red, uint8_t green, uint8_t blue) {
            for (int x = 0; x < MATRIX_WIDTH; x++) {
                for (int y = 0; y < MATRIX_HEIGHT; y++) {
                    SetPixel(x, y, red, green, blue);
                }
            }
        }

        void Clear() {
            Fill(0, 0, 0);
        }

        int width() const {
            return MATRIX_ROWS;
        }

        int height() const {
            return MATRIX_COLS;
        }

    };

    class X11Matrix : public MatrixDevice {
    public:

        GC gc;

        X11Matrix() {
            display = XOpenDisplay(NULL);
            if (display == NULL) {
                fprintf(stderr, "Cannot open display\n");
                exit(1);
            }

            screen = DefaultScreen(display);
            window = XCreateSimpleWindow(display, RootWindow(display, screen), 0, 0, MATRIX_WIDTH,
                                         MATRIX_HEIGHT, 0, BlackPixel(display, screen),
                                         WhitePixel(display, screen));
            XSelectInput(display, window, ExposureMask | KeyPressMask);
            XMapWindow(display, window);
            visual = DefaultVisual(display, screen);

            depth = DefaultDepth(display, screen);
            colormap = DefaultColormap(display, screen);
            XAllocNamedColor(display, colormap, "black", &black, &black);
            XAllocNamedColor(display, colormap, "white", &white, &white);
            XAllocNamedColor(display, colormap, "light gray", &light_gray, &light_gray); // Define light gray color

            // Set background to black
            XSetWindowBackground(display, window, BlackPixel(display, screen));
            XClearWindow(display, window);

            // Create a graphics context
            gc = XCreateGC(display, window, 0, nullptr);
        }

        Canvas *CreateFrameCanvas() {
            return new MockFrameCanvas(display, visual, depth, window);
        }

        void SwapFrameCanvas(Canvas *canvas) {
            // Copy the buffer to the window
            MockFrameCanvas *mockCanvas = (MockFrameCanvas *)(canvas);
            XCopyArea(display, mockCanvas->buffer, window, gc, 0, 0, MATRIX_WIDTH, MATRIX_HEIGHT, 0, 0);

            // Flush graphics requests to the server
            XFlush(display);
        }


        void SetPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue) {
            Logger::logFatal("Cannot call SetPixel on MockMatrix");
        }

        void Fill(uint8_t red, uint8_t green, uint8_t blue) {
            Logger::logFatal("Cannot call Fill on MockMatrix");
        }

        void Close() {
            XCloseDisplay(display);
        }

        void Clear() {
            // Actually clear the window
            XClearWindow(display, window);
        }


    private:
        Display *display;
        Window window;
        int screen;
        Visual *visual;
        int depth;
        Colormap colormap;
        XColor black, white, light_gray;
    };
}

#endif
