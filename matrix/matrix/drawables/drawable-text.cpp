#ifndef TEXT_DRAWER_CPP
#define TEXT_DRAWER_CPP


#include "drawable.h"
#include <vector>
#include <string>
#include <map>

using namespace rgb_matrix;

static std::map<int, rgb_matrix::Font *> fonts; // Preloaded fonts

enum TextScrollingSpeed {
    SLOW = 10000,
    MEDIUM = 5000,
    FAST = 1000
};

class DrawableText : public Drawable {

public:

    DrawableText(MatrixWidget *widget) : Drawable(widget) {

        if (fonts.empty())
        {
            Logger::logFatal("Fonts not loaded, please load fonts before creating a DrawableText object");
        }

        // Load fonts if not already loaded
        //DrawableText::loadFonts();
    }


    void setText(const std::string text) {
        this->text = text;
    }

    void setFontHeight(int fontHeight) {
        this->fontHeight = fontHeight;
    }

    void setScrollingSpeed(TextScrollingSpeed scrollingSpeed) {
        this->scrollingSpeed = scrollingSpeed;
    }

    static void loadFonts() {

        if (!fonts.empty()) return;
        std::vector<std::string> fontFilenames = {
                "4x6.bdf",
                "5x7.bdf",
                "5x8.bdf",
                "6x9.bdf",
                "6x10.bdf",
                "6x12.bdf",
                "6x13.bdf",
                //"7x13.bdf",
                "7x14.bdf",
                //"8x13.bdf",
                "9x15.bdf",
                "9x18.bdf",
                "10x20.bdf"
        };

        for (const auto &filename: fontFilenames) {

            // Extract font height from filename (e.g. 4x6.bdf -> 6)
            int fontHeight = std::stoi(filename.substr(filename.find("x") + 1, filename.find(".bdf")));

            // Load font
            auto *font = new rgb_matrix::Font();
            std::string fontPath = Configs::getFontPath(filename);
            Logger::logDebug("Loading font: " + fontPath);
            if (!font->LoadFont(fontPath.c_str())) {
                Logger::logFatal("Error loading font: " + filename);
            }
            fonts[fontHeight] = font;
        }
    }

private:
    std::string text = "Lorem ipsum";
    int fontHeight = 6;
    TextScrollingSpeed scrollingSpeed = MEDIUM;

    void _draw(Canvas *canvas) override {

        // Get font
        rgb_matrix::Font *font = fonts[fontHeight];
        if (font == nullptr) {
            Logger::logError("Font height " + std::to_string(fontHeight) + " not found, falling back to default font");
            font = fonts[6];
        }

        // Draw text and get text length in pixels
        int textSizePixels = rgb_matrix::DrawText(canvas, *font, xPosition, yPosition + font->baseline(), color,
                                                  text.c_str());

        // If text is too big to be drawn, scroll it
        bool textTooBig = textSizePixels > canvas->width();
        if (textTooBig && !animating) {

            // Check if need to reset position
            if (xPosition <= -textSizePixels) {

                // Reset position
                moveTo(canvas->width(), yPosition);
            }
            else{
                animateTo(-textSizePixels, yPosition, scrollingSpeed);
            }
        }
    }


};

#endif