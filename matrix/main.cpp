#include <thread>
#include "logger/logger.h"
#include "networking/server/tcp-server.cpp"
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

// Tcp server
//TcpServer *tcpServer;

// Signal handler function
void signalHandler(int signal) {
    if (signal == SIGINT || signal == SIGTERM || signal == SIGKILL  || signal == SIGABRT) {
        delete matrix;
        delete bleServer;
        //tcpServer->destroySocket();
        exit(0);
    }
}


// This is run while loading screena is shown
void initStuffBackground() {

    // Fake loading
    //sleep(2);

    // Load fonts in memory to prevent lag when rendering text
    Logger::logInfo("Loading fonts...");
    //DrawableText::loadFonts();

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
    bleServer->startListening();
}

// In background thread, listen for TCP commands
void listenTcp() {
    while (true) {
        try {
           // MatrixSlideshow *slideshowReceived = tcpServer->listenForCommands();

            // Ignore if matrix is not fully initialized
            if (!matrixFullyInitialized) {
                Logger::logDebug("Ignoring new slideshow, matrix not fully initialized");
                continue;
            }

            // Set the new slideshow
            //newSlideshowReceived = slideshowReceived;

        } catch (std::exception &e) {
            Logger::logError("Error downloading slideshow: " + std::string(e.what()));
        }
    }
}



int main(int argc, char *argv[]) {

    // Set up the signal handler
    signal(SIGINT, signalHandler);

    // Initialize the logger
    Logger::initLogger();

    // Start TCP server thread
 //   Logger::logInfo("Starting TCP server...");
   // tcpServer = new TcpServer();
//    std::thread serverThread([&]() {
//        listenTcp();
//    });

    // Init matrix IF YOU MOVE THIS BEFORE TCP SERVER IT CRASHES IDK WHY
    Logger::logInfo("Magic Square is starting");

    // Initialize the matrix
    matrix = MatrixBuilder::build();

 //   tcpServer->matrix = matrix;

    // Show loading at first
    MatrixSlideshow *runningSlideshow = new MatrixSlideshow(matrix);
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