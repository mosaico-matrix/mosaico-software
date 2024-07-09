#include <pybind11/embed.h> // Everything needed for embedding
#include <iostream>

namespace py = pybind11;

void cpp_function() {
    std::cout << "C++ function called from Python!" << std::endl;
}

PYBIND11_MODULE(example, m) {
m.def("cpp_function", &cpp_function, "A function that prints a message");
}

int main() {
    py::scoped_interpreter guard{}; // Start the interpreter and keep it alive

    try {
        py::exec(R"(
import example

def python_function():
    print("Calling C++ function from Python...")
    example.cpp_function()

# Call the function
python_function()
        )");
    } catch (const py::error_already_set& e) {
        std::cerr << "Python error: " << e.what() << std::endl;
    }

    return 0;
}
