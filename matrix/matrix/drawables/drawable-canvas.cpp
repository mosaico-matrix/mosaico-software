#pragma once

#include "drawable.h"

class DrawableCanvas : public Drawable {



public:
    DrawableCanvas () : DrawableShape() {
    }

    void setSize(int w, int h) {
        width = w;
        height = h;
    }

    int getWidth() {
        return width;
    }

    int getHeight() {
        return height;
    }

    void _draw(Canvas *canvas) override {
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                canvas->SetPixel(xPosition + i, yPosition + j, color.r, color.g, color.b);
            }
        }
    }
};