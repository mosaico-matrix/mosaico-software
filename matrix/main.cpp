#include <thread>
#include "logger/logger.h"
#include "python-socket.cpp"
#include "matrix/widget-renderer.cpp"
#include "matrix/matrices/matrix-device.cpp"
#include "matrix/matrices/matrix-builder.cpp"
#include <csignal>
#include "external/json/json.hpp"
#include "matrix/drawables/drawable-text.cpp"

using json = nlohmann::json;

// Global variables
std::map<std::string, rgb_matrix::Font *> DrawableText::fonts;
bool matrixFullyInitialized = false;
WidgetRenderer *runningWidget = nullptr;

// Rgb matrix options
RGBMatrix::Options matrix_options;
rgb_matrix::RuntimeOptions runtime_opt;
MatrixDevice *matrix;

// Python server socket
PythonSocket *pythonSocket;

// Signal handler function
void signalHandler(int signal) {
    if (signal == SIGINT || signal == SIGTERM || signal == SIGKILL || signal == SIGABRT) {
        delete matrix;
        delete pythonSocket;
        delete matrix;
        exit(0);
    }
}

// Draws a simple transition between widgets loading
void drawTransition(Canvas *canvas) {
    // Brightness levels for the squares
    int brightnessLevels[3] = {255, 170, 85};

    // Fixed square size
    int squareSize = 5;

    // Hardcoded positions for the squares centered horizontally and vertically
    // X coordinates of the squares (centered at (32, 32) on the 64x64 matrix)
    int positions[3] = {24, 32, 40}; // Horizontal positions for left, middle, right squares
    int centerY = 32 - (squareSize / 2); // Top-left Y position for all squares

    // Loop to draw the 3 squares
    for (int i = 0; i < 3; ++i) {
        int topLeftX = positions[i] - (squareSize / 2);
        int brightness = brightnessLevels[i];

        // Draw the square as a block of pixels
        for (int x = topLeftX; x < topLeftX + squareSize; ++x) {
            for (int y = centerY; y < centerY + squareSize; ++y) {
                canvas->SetPixel(x, y, brightness, brightness, brightness);
            }
        }
    }
}

// Set a new widget as the active one
void setActiveWidget(WidgetRenderer *newWidget) {

    // Delete the old widget
    Logger::logDebug("Deleting old widget");
    runningWidget = nullptr;
    delete runningWidget;

    // Small loading transition
    auto canvas = matrix->CreateFrameCanvas();
    drawTransition(canvas);
    matrix->SwapFrameCanvas(canvas);

    // Set new widget
    runningWidget = newWidget;
}

// Handle commands received from Python through BLE or COAP
// Note that the socket expects a response so make sure to send a response back at the end of the function
void commandHandler(const std::string &command, const json &data) {

    // Handle different commands
    if (command == "LOAD_WIDGET") {
        try {
            auto newWidget = new WidgetRenderer(matrix);
            newWidget->setDynamicWidget(data["widget_path"], data["config_path"]);
            setActiveWidget(newWidget); // Use the new method to set the widget immediately
        } catch (const std::exception &e) {
            Logger::logError("Error while loading widget: " + std::string(e.what()));
            return;
        }

    } else if (command == "UNLOAD_WIDGET") {
        auto idleWidget = new WidgetRenderer(matrix);
        idleWidget->setIdle();
        setActiveWidget(idleWidget);
    }

    pythonSocket->sendResponse();
}


// This is run while loading screen is shown
void initStuffBackground() {

    // Fake loading
    //sleep(2);

    // Load fonts in memory to prevent lag when rendering text
    Logger::logInfo("Loading fonts...");
    DrawableText::loadFonts();

    // Do other stuff here

    // Set matrix as fully initialized
    matrixFullyInitialized = true;
    Logger::logInfo("Matrix fully initialized");

    // Set IDLE
    auto idleRenderer = new WidgetRenderer(matrix);
    idleRenderer->setIdle();
    setActiveWidget(idleRenderer);

    // Start python server
    pythonSocket = new PythonSocket();
    while (true) {
        auto command = pythonSocket->waitNextCommand();
        commandHandler(command.first, command.second);
    }
}




int main(int argc, char *argv[]) {
    py::initialize_interpreter();

    // Set up the signal handler
    signal(SIGINT, signalHandler);

    // Initialize the logger
    Logger::initLogger();

    // Init matrix
    Logger::logInfo("Mosaico is starting");
    matrix = MatrixBuilder::build();

    // Show loading at first
    runningWidget = new WidgetRenderer(matrix);
    runningWidget->setLoading();

    // Initialize stuff on a separate thread while showing loading
    std::thread loadingThread([&]() {
        initStuffBackground();
    });

    // Main loop
    while (true) {
        // Render the current widget frame
        if (runningWidget != nullptr)
        {
            runningWidget->renderOnMatrix();
        }
    }
}

