#include "../../external/chaiscript/chaiscript.hpp"
#include <iostream>
#include <string>
#include <thread>
using namespace chaiscript;
using namespace std;

class Canvas {
public:
    int a = 5;
    void setPixel()
    {
        cout << "[C++] Setting pixel" << endl;
    }


    void clear()
    {
        cout << "[C++] Clearing canvas" << endl;
    }
};


ChaiScript chai;

int main()
{
    // Load chaiscript from file
    chai.eval_file("test.chai");

    // Create new canvas object
    Canvas *canvas = new Canvas();

    // Register Canvas class and methods (scpoed to canvas object)
    chai.add(user_type<Canvas>(), "Canvas");
    chai.add(constructor<Canvas()>(), "Canvas");
    chai.add(fun(&Canvas::setPixel), "setPixel");
    chai.add(fun(&Canvas::setPixel, canvas), "setPixel");
    chai.add(fun(&Canvas::clear, canvas), "clear");


    while(true)
    {
        // New render
        cout << "[C++] Rendering frame" << endl;

        // Run render frame on chai
        chai.eval("renderFrame()");

        // Sleep
        this_thread::sleep_for(chrono::seconds(1));
    }


    return 0;
}