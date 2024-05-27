#ifndef MATRIX_SLIDESHOW_CPP
#define MATRIX_SLIDESHOW_CPP

#include <unistd.h>
#include <list>
#include "runners/matrix-runner.h"
#include "../networking/client/api-client.cpp"
#include "runners/loading-runner.cpp"
#include "../matrix/matrices/matrix-device.cpp"
#include <thread>
#include "canvas-buffer.cpp"
#include "canvas-layer.cpp"
#include "../networking/client/models.cpp"
#include "runners/dynamic/dynamic-runner.cpp"
#include "runners/dynamic/dynamic-runner-parser.cpp"

using namespace rgb_matrix;

/// The MatrixSlideshow class is responsible for managing the runners and displaying them on the matrix
/// It will display the runners in the order they are added
/// Note that there can be multiple runners displayed at the same time, for example reminders + weather
class MatrixSlideshow {

private:

    std::list<std::pair<int, MatrixRunner*>> runners;   // The actual runners that will be displayed with their order of appearance
    std::vector<MatrixRunner*> currentRunnersGroup;     // The current runners that are displayed
    std::vector<int> framesUntilNextRefresh;            // The frames until the next refresh for each runner [0,5,2] means the first runner will be refreshed in 0 frames, the second in 5 and the third in 2
    int currentRunnerGroupTimeSec           = 0;        // The time the current runner has been running
    int currentRunnerGroupIndex             = 0;        // The index of the current runner
    MatrixDevice *matrix;                               // The matrix
    CanvasBuffer *compositeCanvasBuffer;                // Buffer with frames to prevent flickering

    // Clear all runners and free memory
    void clearRunners() {
        for (const auto& pair : runners) {
            delete pair.second;
        }
        runners.clear();
    }

    // Get the next runners to display in the slideshow
    // Can be multiple since more than one runner can be displayed at the same time
    void loadNextRunnersGroup() {

        // Clear the current runners
        currentRunnersGroup.clear();
        framesUntilNextRefresh.clear();

        // Get the next runners
        for (const auto& pair : runners) {

            // Not the current runner
            if (pair.first != currentRunnerGroupIndex) continue;

            // Add the runner to the next runners
            currentRunnersGroup.push_back(pair.second);
        }

        // Set the frames until the next refresh for each runner
        for (int i = 0; i < currentRunnersGroup.size(); i++) {
            framesUntilNextRefresh.push_back(0);
        }
    }

public:

    MatrixSlideshow(MatrixDevice *matrix) : matrix(matrix)
    {
        compositeCanvasBuffer = new CanvasBuffer(matrix, 2);
    }

    // Destructor
    ~MatrixSlideshow()
    {
        clearRunners();
        delete compositeCanvasBuffer;
    }

    void renderOnMatrix()
    {
        // Record start time
        auto startTime = std::chrono::steady_clock::now();

        // Set the first runner group as the current runners if it's empty
        if (currentRunnersGroup.empty())
        {
            loadNextRunnersGroup();
        }

        // Check which runners need to be refreshed
        for(int runnerIndex = 0; runnerIndex < currentRunnersGroup.size(); runnerIndex++)
        {
            // Check if the runner needs to be refreshed
            if (framesUntilNextRefresh[runnerIndex] == 0)
            {
                // Render the runner on the canvas
                CanvasLayer *partialLayer = currentRunnersGroup[runnerIndex]->renderCanvasLayer();
                compositeCanvasBuffer->paintPartialLayerOnCurrentFrame(partialLayer);

                // Reset the frames until the next refresh
                if(currentRunnersGroup[runnerIndex]->getFrameRate() > 0)
                    framesUntilNextRefresh[runnerIndex] = MATRIX_REFRESH_RATE / currentRunnersGroup[runnerIndex]->getFrameRate() - 1;
                else
                    // If the frame rate is 0, render only once (set to -1 to skip the refresh)
                    framesUntilNextRefresh[runnerIndex] = -1;

            }
            else if (framesUntilNextRefresh[runnerIndex] > 0)
            {
                // Decrement the frames until the next refresh
                framesUntilNextRefresh[runnerIndex]--;

                // Use last rendered frame
                compositeCanvasBuffer->paintPartialLayerOnCurrentFrame(currentRunnersGroup[runnerIndex]->getLastRenderedFrame());
            }
        }

        // Swap the canvas to the matrix
        compositeCanvasBuffer->loadNextFrameOnMatrix();

        // Record end time
        auto endTime = std::chrono::steady_clock::now();

        // Calculate elapsed time
        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

        // Calculate sleep time based on the target frame rate
        int sleepTime = 1000 / MATRIX_REFRESH_RATE - elapsedTime;
        if (sleepTime > 0)
            std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));



        // Adjust target frame rate dynamically if needed
        // For example, change targetFrameRate based on user input or system load
        // Here's just a static example of reducing the frame rate by 5 after every 100 iterations
//        if (currentFrame % 100 == 0 && targetFrameRate > 5)
//            targetFrameRate -= 5;

    }


    /// Set game as the current runner
    /// When setting a game, the other runners are deleted, this must be the only runner
    void setGame(MatrixGameEnum gameType) {

        // Delete all runners
        clearRunners();

        switch (gameType) {
            case MatrixGameEnum::PIXEL_MOVER:
                //runners.push_back(std::make_pair(0, new PixelMover(matrix)));
                break;
            case MatrixGameEnum::TETRIS:
               // runners.push_back(std::make_pair(0, new Tetris()));
                break;
        }
    }

    void setIdle() {
        clearRunners();
        //runners.push_back(std::make_pair(0, new IdleRunner()));
    }

    void showLoading() {
        clearRunners();
        runners.push_back(std::make_pair(0, new LoadingRunner(CanvasLayerPosition::FULL)));
    }

    void setDynamicRunner() {
        clearRunners();

        // Parse sample file
        auto *parser = new DynamicRunnerParser("murkrow","test");
        runners.push_back(std::make_pair(0, new DynamicRunner(parser->getFrameRate(), CanvasLayerPosition::FULL, parser)));
    }

    void setTestRunner() {
        clearRunners();
        //runners.push_back(std::make_pair(0, new FlappyBird()));

//        auto *board = new ApiModels::Board();
//        board->name = "Test";
//        board->boardLines.push_back({1, 1, "Test line 1", false, "#ff0000"});
//        board->boardLines.push_back({2, 1, "Test line 2", true, "#00ff00"});
//        board->boardLines.push_back({3, 1, "Test line 3", false, "#0000ff"});
//        board->boardLines.push_back({3, 1, "Test line 3 aaaaaaas dasd sa", false, "#0000ff"});
//
//        runners.push_back(std::make_pair(0, new BoardRunner(board, CanvasLayerPosition::LEFT)));
//        runners.push_back(std::make_pair(0, new DigitalClockRunner(CanvasLayerPosition::RIGHT)));

//        runners.push_back(std::make_pair(0, new BoardRunner(board, CanvasLayerPosition::LEFT)));
//        runners.push_back(std::make_pair(0, new BoardRunner(board, CanvasLayerPosition::RIGHT)));

//        runners.push_back(std::make_pair(0, new BoardRunner(board, CanvasLayerPosition::BOTTOM_LEFT)));
//        runners.push_back(std::make_pair(0, new BoardRunner(board, CanvasLayerPosition::BOTTOM_RIGHT)));
//        runners.push_back(std::make_pair(0, new BoardRunner(board, CanvasLayerPosition::TOP_LEFT)));
//        runners.push_back(std::make_pair(0, new BoardRunner(board, CanvasLayerPosition::TOP_RIGHT)));


//        runners.push_back(std::make_pair(0, new TestRunner(CanvasLayerPosition::BOTTOM_LEFT, 30)));
//        runners.push_back(std::make_pair(0, new TestRunner(CanvasLayerPosition::BOTTOM_RIGHT, 25)));
//        runners.push_back(std::make_pair(0, new TestRunner(CanvasLayerPosition::TOP_LEFT, 10)));
//        runners.push_back(std::make_pair(0, new TestRunner(CanvasLayerPosition::TOP_RIGHT, 20)));
    }

    void showClock() {
        clearRunners();

//        runners.push_back(std::make_pair(0, new DigitalClockRunner(CanvasLayerPosition::TOP)));
//    runners.push_back(std::make_pair(0, new DigitalClockRunner(CanvasLayerPosition::BOTTOM)));
//        runners.push_back(std::make_pair(0, new DigitalClockRunner(CanvasLayerPosition::BOTTOM_LEFT)));
//        runners.push_back(std::make_pair(0, new DigitalClockRunner(CanvasLayerPosition::BOTTOM_RIGHT)));
//        runners.push_back(std::make_pair(0, new DigitalClockRunner(CanvasLayerPosition::TOP_LEFT)));
//        runners.push_back(std::make_pair(0, new DigitalClockRunner(CanvasLayerPosition::TOP_RIGHT)));
    }

};

#endif