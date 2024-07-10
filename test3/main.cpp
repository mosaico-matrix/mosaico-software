#include <pybind11/embed.h>
#include <iostream>

namespace py = pybind11;

// Example C++ function
void my_cpp_function() {
    std::cout << "Hello from C++!" << std::endl;
}

int main() {
    py::scoped_interpreter guard{};

    // Call your C++ function
    my_cpp_function();

    // Example Python interaction
    py::exec("print('Hello from Python!')");

    return 0;
}
