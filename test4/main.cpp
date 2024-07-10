#include <pybind11/embed.h>
#include <pybind11/pybind11.h>
#include <iostream>
#include <string>

namespace py = pybind11;


class Widget {
public:

    void test() {
        std::cout << "gg" << std::endl;
    }

    py::scoped_interpreter guard{};

    Widget()
    {
        // Expose the instance to Python
        py::module mosaico = py::module::import("mosaico");
        mosaico.attr("widget") = py::cast(this);

        // Example of executing Python code with py::exec that uses the created instance
        py::exec(R"(
        from mosaico import widget, Text

        obj = Text(widget)
    )");
    }
};

class Text {
public:
    Text(Widget *widget)
    {
        widget->test();
    }
};


PYBIND11_EMBEDDED_MODULE(mosaico, m) {

py::class_<Widget>(m, "Widget")
.def(py::init<>())
.def("test", &Widget::test);

py::class_<Text>(m, "Text")
.def(py::init<Widget*>());

}



int main() {


    auto widget = Widget();

    return 0;
}

