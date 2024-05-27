#ifndef DRAWABLE_PPM_CPP
#define DRAWABLE_PPM_CPP

#include "drawable.h"
#include <stdio.h>
#include <math.h>
#include <stdio.h>
#include <unistd.h>

class DrawablePPM : public Drawable {

private:

    struct Image {
        Image() : width(-1), height(-1), image(NULL) {}

        ~Image() { Delete(); }

        void Delete() {
            delete[] image;
            Reset();
        }

        void Reset() {
            image = NULL;
            width = -1;
            height = -1;
        }

        inline bool IsValid() { return image && height > 0 && width > 0; }

        const Pixel &getPixel(int x, int y) {
            static Pixel black;
            if (x < 0 || x >= width || y < 0 || y >= height) return black;
            return image[x + width * y];
        }

        int width;
        int height;
        Pixel *image;
    };

    Image image;

    // Read line, skip comments from file
    char *ReadLine(FILE *f, char *buffer, size_t len) {
        char *result;
        do {
            result = fgets(buffer, len, f);
        } while (result != NULL && result[0] == '#');
        return result;
    }

public:
    DrawablePPM(const char *filename, MatrixRunner *runner) : Drawable(runner) {

        // Search in the images folder
//        std::string filePath = Configs::getImagePath(filename);
        FILE *f = fopen(filename, "r");

        // check if file exists
        if (f == NULL && access(filename, F_OK) == -1) {
            fprintf(stderr, "File \"%s\" doesn't exist\n", filename);
            return;
        }

        if (f == NULL) return;

        char header_buf[256];
        const char *line = ReadLine(f, header_buf, sizeof(header_buf));

#define EXIT_WITH_MSG(m) { fprintf(stderr, "%s: %s |%s", filename, m, line); fclose(f); return; }

        if (sscanf(line, "P6 ") == EOF) EXIT_WITH_MSG("Can only handle P6 as PPM type.");

        line = ReadLine(f, header_buf, sizeof(header_buf));
        int new_width, new_height;
        if (!line || sscanf(line, "%d %d ", &new_width, &new_height) != 2) EXIT_WITH_MSG("Width/height expected");

        int value;
        line = ReadLine(f, header_buf, sizeof(header_buf));
        if (!line || sscanf(line, "%d ", &value) != 1 || value != 255) EXIT_WITH_MSG("Only 255 for maxval allowed.");

        const size_t pixel_count = new_width * new_height;
        Pixel *new_image = new Pixel[pixel_count];

        if (fread(new_image, sizeof(Pixel), pixel_count, f) != pixel_count) {
            line = "";
            EXIT_WITH_MSG("Not enough pixels read.");
        }

#undef EXIT_WITH_MSG


        fclose(f);
        image.Delete();  // in case we reload faster than is picked up
        image.image = new_image;
        image.width = new_width;
        image.height = new_height;
    }

    void _draw(Canvas *canvas) override {
        // Actually draw the image
        for (int x = 0; x < canvas->width(); ++x) {
            for (int y = 0; y < canvas->height(); ++y) {
                const Pixel &p = image.getPixel(
                        (x) % image.width, y);
                canvas->SetPixel(x, y, p.red, p.green, p.blue);
            }
        }
    }
};

#endif
//
//#include "drawable.cpp"
//#include <assert.h>
//#include <math.h>
//#include <stdio.h>
//#include <unistd.h>
//#include <algorithm>
//#include "../../configs.cpp"
//#include "../../logger/logger.h"
//#include "../matrix-runner.cpp"
//
//class MatrixRunner;
//class DrawablePPM : public Drawable {
//
//private:
//
//    struct Pixel {
//        Pixel() : red(0), green(0), blue(0) {}
//
//        uint8_t red;
//        uint8_t green;
//        uint8_t blue;
//    };
//
//    struct Image {
//        Image() : width(-1), height(-1), image(NULL) {}
//
//        ~Image() { Delete(); }
//
//        void Delete() {
//            delete[] image;
//            Reset();
//        }
//
//        void Reset() {
//            image = NULL;
//            width = -1;
//            height = -1;
//        }
//
//        inline bool IsValid() { return image && height > 0 && width > 0; }
//
//        const Pixel &getPixel(int x, int y) {
//            static Pixel black;
//            if (x < 0 || x >= width || y < 0 || y >= height) return black;
//            return image[x + width * y];
//        }
//
//        int width;
//        int height;
//        Pixel *image;
//    };
//
//    Image image;
//    // Read line, skip comments from file
//    char *ReadLine(FILE *f, char *buffer, size_t len) {
//        char *result;
//        do {
//            result = fgets(buffer, len, f);
//        } while (result != NULL && result[0] == '#');
//        return result;
//    }
//public:
//    DrawablePPM(const char *filename, MatrixRunner *runner) : Drawable(runner){
//
//        // Search in the images folder
//        std::string filePath = Configs::getImagePath(filename);
//        Logger::logDebug("Loading image: " + filePath);
//        FILE *f = fopen(filePath.c_str(), "r");
//
//        // check if file exists
//        if (f == NULL && access(filePath.c_str(), F_OK) == -1) {
//            fprintf(stderr, "File \"%s\" doesn't exist\n", filePath.c_str());
//            return;
//        }
//
//        if (f == NULL) return;
//
//        char header_buf[256];
//        const char *line = ReadLine(f, header_buf, sizeof(header_buf));
//
//#define EXIT_WITH_MSG(m) { fprintf(stderr, "%s: %s |%s", filename, m, line); fclose(f); return; }
//
//        if (sscanf(line, "P6 ") == EOF) EXIT_WITH_MSG("Can only handle P6 as PPM type.");
//
//        line = ReadLine(f, header_buf, sizeof(header_buf));
//        int new_width, new_height;
//        if (!line || sscanf(line, "%d %d ", &new_width, &new_height) != 2) EXIT_WITH_MSG("Width/height expected");
//
//        int value;
//        line = ReadLine(f, header_buf, sizeof(header_buf));
//        if (!line || sscanf(line, "%d ", &value) != 1 || value != 255) EXIT_WITH_MSG("Only 255 for maxval allowed.");
//
//        const size_t pixel_count = new_width * new_height;
//        Pixel *new_image = new Pixel[pixel_count];
//
//        assert(sizeof(Pixel) == 3);   // we make that assumption.
//        if (fread(new_image, sizeof(Pixel), pixel_count, f) != pixel_count) {
//            line = "";
//            EXIT_WITH_MSG("Not enough pixels read.");
//        }
//
//#undef EXIT_WITH_MSG
//
//
//        fclose(f);
//        image.Delete();  // in case we reload faster than is picked up
//        image.image = new_image;
//        image.width = new_width;
//        image.height = new_height;
//    }
//
//
//    void _draw(Canvas *canvas) override {
//        // Actually draw the image
//        for (int x = 0; x < canvas->width(); ++x) {
//            for (int y = 0; y < canvas->height(); ++y) {
//                const Pixel &p = image.getPixel(
//                        (x) % image.width, y);
//                canvas->SetPixel(x, y, p.red, p.green, p.blue);
//            }
//        }
//    }
//
//
//};
//
//#endif
//
