#ifndef DRAWABLE_PPM_CPP
#define DRAWABLE_PPM_CPP

#include "drawable.h"
#include <unistd.h>

class DrawablePPM : public Drawable {

private:

    struct Image {
        Image() : width(-1), height(-1), image(nullptr) {}

        ~Image() { Delete(); }

        void Delete() {
            delete[] image;
            Reset();
        }

        void Reset() {
            image = nullptr;
            width = -1;
            height = -1;
        }

        [[nodiscard]] inline bool IsValid() const { return image && height > 0 && width > 0; }

        [[nodiscard]] const Pixel &getPixel(int x, int y) const {
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
    static char *ReadLine(FILE *f, char *buffer, size_t len) {
        char *result;
        do {
            result = fgets(buffer, len, f);
        } while (result != NULL && result[0] == '#');
        return result;
    }

public:
    explicit DrawablePPM(const char *filename) {

        FILE *f = fopen(filename, "r");

        // check if file exists
        if (f == nullptr && access(filename, F_OK) == -1) {
            fprintf(stderr, "File \"%s\" doesn't exist\n", filename);
            return;
        }

        if (f == nullptr) return;

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