#pragma once

#include <pybind11/embed.h>
#include "../../../drawables/colors.cpp"

namespace py = pybind11;

class ColorsModule {
public:
    static void load(py::module *m) {

        // Bind the colors in 'colors' module
        py::module colors = m->def_submodule("colors");
        colors.attr("red") = RED_COLOR;
        colors.attr("green") = GREEN_COLOR;
        colors.attr("blue") = BLUE_COLOR;
        colors.attr("white") = WHITE_COLOR;
        colors.attr("black") = BLACK_COLOR;
        colors.attr("yellow") = YELLOW_COLOR;
        colors.attr("cyan") = CYAN_COLOR;
        colors.attr("magenta") = MAGENTA_COLOR;
        colors.attr("orange") = ORANGE_COLOR;
        colors.attr("dark_orange") = DARK_ORANGE_COLOR;
        colors.attr("purple") = PURPLE_COLOR;
        colors.attr("pink") = PINK_COLOR;
        colors.attr("lime") = LIME_COLOR;
        colors.attr("brown") = BROWN_COLOR;
        colors.attr("maroon") = MAROON_COLOR;
        colors.attr("olive") = OLIVE_COLOR;
        colors.attr("navy") = NAVY_COLOR;
    }
};