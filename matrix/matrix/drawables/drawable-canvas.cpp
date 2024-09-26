#pragma once

#include "drawable.h"
#include "../canvas-layer.cpp"

class DrawableCanvas : public Drawable {

private:
    CanvasLayer *canvasLayer;

public:
    DrawableCanvas (CanvasLayer* existingLayer = nullptr) : Drawable() {
        canvasLayer = existingLayer == nullptr ? new CanvasLayer() : existingLayer;
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

    void setPixel(int x, int y, Color color) {
        canvasLayer->SetPixel(x, y, color);
    }

    void clear() {
        canvasLayer->Clear();
    }

    void fill(Color color) {
        canvasLayer->Fill(color);
    }

    DrawableCanvas* clone() {
        return new DrawableCanvas(canvasLayer->Clone());
    }

    void _draw(Canvas *canvas) override {
        canvasLayer->paintOntoCanvas(canvas, xPosition, yPosition);
    }
};