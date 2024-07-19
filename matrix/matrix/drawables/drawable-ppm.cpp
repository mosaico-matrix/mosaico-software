#ifndef DRAWABLE_PPM_CPP
#define DRAWABLE_PPM_CPP

#include "drawable.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>
#include <stdexcept>
#include "../../configs.cpp"
class DrawablePPM : public Drawable {
private:
    struct Image {
        int width;
        int height;
        std::unique_ptr<Pixel[]> pixels;

        Image() : width(-1), height(-1) {}

        void reset(int w, int h) {
            width = w;
            height = h;
            pixels = std::make_unique<Pixel[]>(w * h);
        }

        bool isValid() const {
            return pixels != nullptr && width > 0 && height > 0;
        }

        const Pixel &getPixel(int x, int y) const {
            static Pixel black;
            if (x < 0 || x >= width || y < 0 || y >= height) return black;
            return pixels[x + width * y];
        }
    };

    Image image;
    static std::vector<std::string> split(const std::string &str, char delimiter) {
        std::vector<std::string> tokens;
        std::stringstream ss(str);
        std::string token;
        while (std::getline(ss, token, delimiter)) {
            tokens.push_back(token);
        }
        return tokens;
    }

    void loadP6(std::ifstream &file) {
        std::string line;
        std::getline(file, line);
        if (line != "P6") throw std::runtime_error("Can only handle P6 as PPM type.");

        while (std::getline(file, line) && line[0] == '#');  // Skip comments
        auto dims = split(line, ' ');
        if (dims.size() != 2) throw std::runtime_error("Width/height expected");
        int new_width = std::stoi(dims[0]);
        int new_height = std::stoi(dims[1]);

        while (std::getline(file, line) && line[0] == '#');  // Skip comments
        if (std::stoi(line) != 255) throw std::runtime_error("Only 255 for maxval allowed.");

        size_t pixel_count = new_width * new_height;
        image.reset(new_width, new_height);
        file.read(reinterpret_cast<char*>(image.pixels.get()), pixel_count * sizeof(Pixel));
        if (!file) throw std::runtime_error("Not enough pixels read.");
    }

    void loadP3(std::ifstream &file) {
        std::string line;
        std::getline(file, line);
        if (line != "P3") throw std::runtime_error("Can only handle P3 as PPM type.");

        while (std::getline(file, line) && line[0] == '#');  // Skip comments
        auto dims = split(line, ' ');
        if (dims.size() != 2) throw std::runtime_error("Width/height expected");
        int new_width = std::stoi(dims[0]);
        int new_height = std::stoi(dims[1]);

        while (std::getline(file, line) && line[0] == '#');  // Skip comments
        if (std::stoi(line) != 255) throw std::runtime_error("Only 255 for maxval allowed.");

        size_t pixel_count = new_width * new_height;
        image.reset(new_width, new_height);

        for (size_t i = 0; i < pixel_count; ++i) {
            int r, g, b;
            file >> r >> g >> b;
            image.pixels[i] = Pixel(r, g, b);
        }
    }

public:
    explicit DrawablePPM() {}

    void setImage(const char *filename) {
        std::ifstream file(filename, std::ios::binary);
        if (!file) {
            throw std::runtime_error(std::string("File \"") + filename + "\" doesn't exist");
        }

        // Load PPM file
        std::string header;
        std::getline(file, header);
        file.seekg(0);
        if (header == "P6") {
            loadP6(file);
        } else if (header == "P3") {
            loadP3(file);
        } else {
            throw std::runtime_error("Unsupported PPM format");
        }
    }

    void _draw(Canvas *canvas) override {
        if (!image.isValid()) {
            // Load default image
            setImage(Configs::getImagePath("heart.ppm").c_str());
        }

        for (int x = 0; x < image.width; ++x) {
            for (int y = 0; y < image.height; ++y) {
                const Pixel &p = image.getPixel(x, y);
                canvas->SetPixel(x + xPosition, y + yPosition, p.red, p.green, p.blue);
            }
        }
    }

};

#endif
