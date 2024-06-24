#include <thread>
#include "logger/logger.h"
#include "python-socket.cpp"
#include "matrix/matrix-slideshow.cpp"
#include "matrix/matrices/matrix-device.cpp"
#include "matrix/matrices/matrix-builder.cpp"
#include <csignal>
#include "external/json/json.hpp"
using json = nlohmann::json;

// Global variables
bool matrixFullyInitialized = false;
MatrixSlideshow *newSlideshowReceived = NULL;

// Rgb matrix options
RGBMatrix::Options matrix_options;
rgb_matrix::RuntimeOptions runtime_opt;
MatrixDevice *matrix;

// Python server socket
PythonSocket *pythonSocket;

// Signal handler function
void signalHandler(int signal) {
    if (signal == SIGINT || signal == SIGTERM || signal == SIGKILL  || signal == SIGABRT) {
        delete matrix;
        delete pythonSocket;
        exit(0);
    }
}

// Handle commands received from Python through BLE
// Note that the socket expects a response so make sure to send a response back at the end of the function
void commandHandler(const std::string& command, const json &data) {

    // Handle different commands
    if (command == "LOAD_WIDGET") {

        // Create new slideshow
        newSlideshowReceived = new MatrixSlideshow(matrix);
        newSlideshowReceived->setDynamicWidget(data["widget_path"], data["config_path"]);

    } else if (command == "CMD2") {
        // Do something with data for command 2
        std::cout << "Received CMD2 with data: " << data << std::endl;
        // Send response back to Python
        pythonSocket->sendResponse("Response to CMD2");
    } else {
        // Unknown command
        std::cerr << "Unknown command: " << command << std::endl;
        // Send error response back to Python
        pythonSocket->sendResponse("Unknown command");
    }

    pythonSocket->sendResponse();
}


// This is run while loading screena is shown
void initStuffBackground() {

    // Fake loading
    //sleep(2);

    // Load fonts in memory to prevent lag when rendering text
    Logger::logInfo("Loading fonts...");
    DrawableText::loadFonts();

    // Set matrix as fully initialized
    matrixFullyInitialized = true;
    Logger::logInfo("Matrix fully initialized");

    // Set the new slideshow
    newSlideshowReceived = new MatrixSlideshow(matrix);
  //  newSlideshowReceived->setDynamicRunner();
//  newSlideshowReceived->setIdle();
//  newSlideshowReceived->showClock();
//  newSlideshowReceived->setTestRunner();
//  newSlideshowReceived->setGame(MatrixGameEnum::TETRIS);
    newSlideshowReceived->showLoading();

    // Start python server
    pythonSocket = new PythonSocket();
    while (true)
    {
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
    auto *runningSlideshow = new MatrixSlideshow(matrix);
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
        if (newSlideshowReceived != NULL) {

            // Delete current slideshow
            delete runningSlideshow;

            runningSlideshow = newSlideshowReceived;

            // Reset newSlideshowReceived
            newSlideshowReceived = NULL;

            // Clear the matrix
            //matrix->Clear();
        }
    }
}