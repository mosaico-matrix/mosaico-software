#include <thread>
#include "logger/logger.h"
#include "networking/server/ble/ble-server.cpp"
#include "matrix/matrix-slideshow.cpp"
#include "matrix/matrices/matrix-device.cpp"
#include "matrix/matrices/matrix-builder.cpp"
#include <csignal>

// Global variables
bool matrixFullyInitialized = false;
MatrixSlideshow *newSlideshowReceived = NULL;

// Rgb matrix options
RGBMatrix::Options matrix_options;
rgb_matrix::RuntimeOptions runtime_opt;
MatrixDevice *matrix;

// BLE server
BleServer *bleServer;

// Signal handler function
void signalHandler(int signal) {
    if (signal == SIGINT || signal == SIGTERM || signal == SIGKILL  || signal == SIGABRT) {
        delete matrix;
        delete bleServer;
        exit(0);
    }
}

// Handle commands received from Python through BLE
void commandHandler(const std::string& command, const std::string& data) {

    // Handle different commands
    if (command == "LOAD_WIDGET") {

        // Create new slideshow
        newSlideshowReceived = new MatrixSlideshow(matrix);
        newSlideshowReceived->setDynamicRunner(data);

        // Send response back to Python
        bleServer->sendResponse("Response to CMD1");
    } else if (command == "CMD2") {
        // Do something with data for command 2
        std::cout << "Received CMD2 with data: " << data << std::endl;
        // Send response back to Python
        bleServer->sendResponse("Response to CMD2");
    } else {
        // Unknown command
        std::cerr << "Unknown command: " << command << std::endl;
        // Send error response back to Python
        bleServer->sendResponse("Unknown command");
    }
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

    // Start BLE server
    bleServer = new BleServer();
    while (true)
    {
        auto command = bleServer->waitNextCommand();
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