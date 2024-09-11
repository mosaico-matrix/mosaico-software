#pragma once

#include "../../external/rpi-rgb-led-matrix/include/graphics.h"
#include <cstdlib>
#include <vector>


using namespace rgb_matrix;

static const Color RED_COLOR = Color(255, 0, 0);
static const Color GREEN_COLOR = Color(0, 255, 0);
static const Color BLUE_COLOR = Color(0, 0, 255);
static const Color WHITE_COLOR = Color(255, 255, 255);
static const Color BLACK_COLOR = Color(0, 0, 0);
static const Color YELLOW_COLOR = Color(255, 255, 0);
static const Color CYAN_COLOR = Color(0, 255, 255);
static const Color MAGENTA_COLOR = Color(255, 0, 255);
static const Color ORANGE_COLOR = Color(255, 165, 0);
static const Color DARK_ORANGE_COLOR = Color(255, 100, 0);
static const Color PURPLE_COLOR = Color(128, 0, 128);
static const Color PINK_COLOR = Color(255, 192, 203);
static const Color LIME_COLOR = Color(0, 255, 0);
static const Color BROWN_COLOR = Color(165, 42, 42);
static const Color MAROON_COLOR = Color(128, 0, 0);
static const Color OLIVE_COLOR = Color(128, 128, 0);
static const Color NAVY_COLOR = Color(0, 0, 128);

static const std::vector<Color> COLORS = {
        RED_COLOR,
        GREEN_COLOR,
        BLUE_COLOR,
        WHITE_COLOR,
        YELLOW_COLOR,
        CYAN_COLOR,
        MAGENTA_COLOR,
        ORANGE_COLOR,
        DARK_ORANGE_COLOR,
};

static const Color RandomColor() {
    return COLORS[rand() % COLORS.size()];
}
