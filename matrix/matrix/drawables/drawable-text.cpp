#ifndef TEXT_DRAWER_CPP
#define TEXT_DRAWER_CPP

#include "drawable.h"
#include <vector>
#include <string>
#include <map>

using namespace rgb_matrix;

enum TextScrollingSpeed {
    SLOW = 10000,
    MEDIUM = 5000,
    FAST = 1000
};


class DrawableText : public Drawable {

public:
    static std::map<std::string, rgb_matrix::Font *> fonts; // Preloaded fonts

    DrawableText() : Drawable() {
        if (DrawableText::fonts.empty())
        {
            Logger::logFatal("Fonts not loaded, please load fonts before creating a DrawableText object");
        }
    }

    void setText(const std::string text) {
        this->text = text;
    }

    void setFont(std::string font) {
        this->fontName = font;
    }

    void setScrollingSpeed(TextScrollingSpeed scrollingSpeed) {
        this->scrollingSpeed = scrollingSpeed;
    }

    static void loadFonts() {

        if (!DrawableText::fonts.empty()) return;
        std::vector<std::string> fontNames = {
                "4x6",
                "5x7",
                "5x8",
                "6x9",
                "6x10",
                "6x12",
                "6x13",
                "7x13",
                "7x14",
                "8x13",
                "9x15",
                "9x18",
                "10x20",
                "clR6x12",
                "7x14B",
                "9x15B",
                "texgyre-27",
                "8x13O",
                "7x13B",
                "9x18B",
                "6x13O",
                "tom-thumb",
                "helvR12",
                "6x13B",
                "7x13O",
                "8x13B"
        };

        for (const auto &fontName: fontNames) {

            // Load font
            auto *font = new rgb_matrix::Font();
            std::string fontPath = Configs::getFontPath(fontName + ".bdf");
            Logger::logDebug("Loading font: " + fontPath);
            if (!font->LoadFont(fontPath.c_str())) {
                Logger::logFatal("Error loading font: " + fontName);
            }
            DrawableText::fonts[fontName] = font;
        }
    }

private:
    std::string text = "";
    std::string fontName = "5x7";
    TextScrollingSpeed scrollingSpeed = MEDIUM;

    void _draw(Canvas *canvas) override {

        // Get font
        rgb_matrix::Font *font = DrawableText::fonts[fontName];
        if (font == nullptr) {
            Logger::logError("Font height " + fontName + " not found, falling back to default font");
            font = DrawableText::fonts["5x7"];
        }

        // Draw text and get text length in pixels
        int textSizePixels = rgb_matrix::DrawText(canvas, *font, xPosition, yPosition + font->baseline(), color,
                                                  text.c_str());

        // Update width
        width = textSizePixels > 64 ? 64 : textSizePixels;

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
