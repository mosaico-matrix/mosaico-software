#include <pybind11/embed.h>
#include "../dynamic-widget.cpp"
#include "../../matrix-widget.h"
#include "../../../drawables/drawable.h"
#include "../../../drawables/drawable-text.cpp"

namespace py = pybind11;

PYBIND11_EMBEDDED_MODULE(mosaico, m) {

    // Binding the MatrixWidget class
    py::class_<MatrixWidget>(m, "MatrixWidget")
    .def("createRectangle", &MatrixWidget::createRectangle)
    .def("createText", &MatrixWidget::createText);

    // Binding the DynamicWidget class
    py::class_<DynamicWidget, MatrixWidget>(m, "DynamicWidget");

    // Bind the Drawable class
    py::class_<Drawable>(m, "Drawable")
    .def("moveTo", &Drawable::moveTo)
    .def("translate", &Drawable::translate)
    .def("translateX", &Drawable::translateX)
    .def("translateY", &Drawable::translateY)
    .def("setColor", &Drawable::setColor)
    .def("animateTo", &Drawable::animateTo)
    .def("hide", &Drawable::hide)
    .def("show", &Drawable::show)
    .def("isVisible", &Drawable::isVisible)
    .def("isAnimating", &Drawable::isAnimating)
    .def("getX", &Drawable::getX)
    .def("getY", &Drawable::getY);


    // Bind the DrawableShape class
    py::class_<DrawableShape, Drawable>(m, "DrawableShape");

    // Bind the DrawableRectangle class
    py::class_<DrawableRectangle, DrawableShape>(m, "DrawableRectangle")
    .def("setSize", &DrawableRectangle::setSize);

    // Bind the DrawableText class
    py::class_<DrawableText, Drawable>(m, "DrawableText")
    .def("setText", &DrawableText::setText)
    .def("setFontHeight", &DrawableText::setFontHeight)
    .def("setScrollingSpeed", &DrawableText::setScrollingSpeed)
    .def("loadFonts", &DrawableText::loadFonts);

}
