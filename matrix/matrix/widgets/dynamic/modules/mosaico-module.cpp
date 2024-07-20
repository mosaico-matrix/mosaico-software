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
            .def("createImage", &MatrixWidget::createPPM)
            .def("createText", &MatrixWidget::createText);

    // Binding the DynamicWidget class
    py::class_<DynamicWidget, MatrixWidget>(m, "DynamicWidget")
            .def("widgetAsset", &DynamicWidget::widgetAssetPath)
            .def("configAsset", &DynamicWidget::configAssetPath);

    // Bind the Drawable class
    py::class_<Drawable>(m, "Drawable")
    .def("moveTo", &Drawable::moveTo)
    .def("translateBy", &Drawable::translateBy)
    .def("translateXBy", &Drawable::translateXBy)
    .def("translateYBy", &Drawable::translateYBy)
    .def("setColor", &Drawable::setColor)
    .def("setHexColor", &Drawable::setHexColor)
    .def("animateTo", &Drawable::animateTo)
    .def("hide", &Drawable::hide)
    .def("show", &Drawable::show)
    .def("isVisible", &Drawable::isVisible)
    .def("isAnimating", &Drawable::isAnimating)
    .def("getX", &Drawable::getX)
    .def("getY", &Drawable::getY)
    .def("centerHorizontally", &Drawable::centerHorizontally);


    // Bind the DrawableShape class
    py::class_<DrawableShape, Drawable>(m, "DrawableShape");

    // Bind the DrawableRectangle class
    py::class_<DrawableRectangle, DrawableShape>(m, "DrawableRectangle")
    .def("getWidth", &DrawableRectangle::getWidth)
    .def("getHeight", &DrawableRectangle::getHeight)
    .def("setSize", &DrawableRectangle::setSize);

    // Bind the DrawableText class
    py::class_<DrawableText, Drawable>(m, "DrawableText")
    .def("setText", &DrawableText::setText)
    .def("setFont", &DrawableText::setFont)
    .def("setScrollingSpeed", &DrawableText::setScrollingSpeed);

    // Bind the DrawablePPM class
    py::class_<DrawablePPM, Drawable>(m, "DrawablePPM")
    .def("setImage", &DrawablePPM::setImage)
    .def("flipVertically", &DrawablePPM::flipVertically)
    .def("flipHorizontally", &DrawablePPM::flipHorizontally);

    // Bind color class
    py::class_<Color>(m, "Color")
    .def(py::init<int, int, int>());


}

