#ifndef DRAWABLE_SHAPE_CPP
#define DRAWABLE_SHAPE_CPP

#include "drawable.h"

class DrawableShape : public Drawable {

public:
    DrawableShape(MatrixWidget *runner) : Drawable(runner) {}
};


class DrawableRectangle : public DrawableShape {

private:
    int width = 5;
    int height = 5;

public:
    DrawableRectangle (MatrixWidget *runner) : DrawableShape(runner) {
    }

    void setSize(int w, int h) {
        width = w;
        height = h;
    }

    void _draw(Canvas *canvas) override {
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                canvas->SetPixel(xPosition + i, yPosition + j, color.r, color.g, color.b);
            }
        }
    }
};

#endif
