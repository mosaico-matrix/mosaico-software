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
std::map<int, rgb_matrix::Font *> DrawableText::fonts;
bool matrixFullyInitialized = false;
WidgetRenderer *newWidgetReceived = NULL;

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
        exit(0);
    }
}

// Handle commands received from Python through BLE or COAP
// Note that the socket expects a response so make sure to send a response back at the end of the function
void commandHandler(const std::string &command, const json &data) {

    // Handle different commands
    if (command == "LOAD_WIDGET") {
        try {
            auto newWidget = new WidgetRenderer(matrix);
            newWidget->setDynamicWidget(data["widget_path"], data["config_path"]);
            newWidgetReceived = newWidget;
        } catch (const std::exception &e) {
            Logger::logError("Error while loading widget: " + std::string(e.what()));
            return;
        }

    } else if (command == "UNLOAD_WIDGET") {
        newWidgetReceived = new WidgetRenderer(matrix);
        newWidgetReceived->setIdle();

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

    // Set the new widget
    newWidgetReceived = new WidgetRenderer(matrix);
    newWidgetReceived->showLoading();

    // Start python server
    pythonSocket = new PythonSocket();
    while (true) {
        auto command = pythonSocket->waitNextCommand();
        commandHandler(command.first, command.second);
    }
}


int main(int argc, char *argv[]) {

    // Set up the signal handler
    signal(SIGINT, signalHandler);

    // Initialize the logger
    Logger::initLogger();

    // Init matrix
    Logger::logInfo("Mosaico is starting");
    matrix = MatrixBuilder::build();

    // Show loading at first
    auto *runningSlideshow = new WidgetRenderer(matrix);
    runningSlideshow->showLoading();

    // Initialize stuff on a separate thread while showing loading
    std::thread loadingThread([&]() {
        initStuffBackground();
    });

    // Main loop
    while (true) {

        // Render the current slideshow frame
        runningSlideshow->renderOnMatrix();

        // Check if a new slideshow was received
        if (newWidgetReceived != NULL) {

            // Delete current slideshow
            delete runningSlideshow;

            runningSlideshow = newWidgetReceived;

            // Reset newSlideshowReceived
            newWidgetReceived = NULL;

            // Clear the matrix
            //matrix->Clear();
        }
    }
}