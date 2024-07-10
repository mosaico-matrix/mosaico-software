#ifndef MATRIX_SLIDESHOW_CPP
#define MATRIX_SLIDESHOW_CPP

#include <unistd.h>
#include <list>
#include "widgets/matrix-widget.h"
#include "../networking/client/api-client.cpp"
#include "widgets/loading-widget.cpp"
#include "../matrix/matrices/matrix-device.cpp"
#include <thread>
#include "canvas-buffer.cpp"
#include "canvas-layer.cpp"
#include "../networking/client/models.cpp"
#include "widgets/dynamic/dynamic-widget.cpp"

using namespace rgb_matrix;

#define CANVAS_BUFFER_SIZE 2

/// The MatrixSlideshow class is responsible for managing the runners and displaying them on the matrix
/// It will display the runners in the order they are added
/// Note that there can be multiple runners displayed at the same time, for example reminders + weather
class WidgetRenderer {

private:

    std::vector<MatrixWidget*> currentWidgetsGroup;     // The current widgets that are displayed
    std::vector<int> framesUntilNextRefresh;            // The frames until the next refresh for each widget [0,5,2] means the first widget will be refreshed in 0 frames, the second in 5 and the third in 2
    MatrixDevice *matrix;                               // The matrix
    CanvasBuffer *compositeCanvasBuffer;                // Buffer with frames to prevent flickering

public:

    explicit WidgetRenderer(MatrixDevice *matrix) : matrix(matrix)
    {
        compositeCanvasBuffer = new CanvasBuffer(matrix, CANVAS_BUFFER_SIZE);
    }

    // Destructor
    ~WidgetRenderer()
    {
        delete compositeCanvasBuffer;
        for (auto widget : currentWidgetsGroup)
        {
            delete widget;
        }
    }

    void renderOnMatrix()
    {
        // Record start time
        auto startTime = std::chrono::steady_clock::now();

        // Check which widgets need to be refreshed
        for(int widgetIndex = 0; widgetIndex < currentWidgetsGroup.size(); widgetIndex++)
        {
            // Check if the runner needs to be refreshed
            if (framesUntilNextRefresh[widgetIndex] == 0)
            {
                // Render the runner on the canvas
                CanvasLayer *partialLayer = currentWidgetsGroup[widgetIndex]->renderCanvasLayer();
                compositeCanvasBuffer->paintPartialLayerOnCurrentFrame(partialLayer);

                // Reset the frames until the next refresh
                if(currentWidgetsGroup[widgetIndex]->getFrameRate() > 0)
                    framesUntilNextRefresh[widgetIndex] = MATRIX_REFRESH_RATE / currentWidgetsGroup[widgetIndex]->getFrameRate() - 1;
                else
                    // If the frame rate is 0, render only once (set to -1 to skip the refresh)
                    framesUntilNextRefresh[widgetIndex] = -1;

            }
            else if (framesUntilNextRefresh[widgetIndex] > 0)
            {
                // Decrement the frames until the next refresh
                framesUntilNextRefresh[widgetIndex]--;

                // Use last rendered frame
                compositeCanvasBuffer->paintPartialLayerOnCurrentFrame(currentWidgetsGroup[widgetIndex]->getLastRenderedFrame());
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


    void setLoading() {
        framesUntilNextRefresh.push_back(0);
        currentWidgetsGroup = {new LoadingWidget()};
    }

    void setDynamicWidget(const string& widgetDirPath, const string& configDirPath) {
        framesUntilNextRefresh.push_back(0); // TODO: this should be adjusted when using more than one widget
        currentWidgetsGroup = {new DynamicWidget(widgetDirPath, configDirPath)};
    }

    void setIdle()
    {
        framesUntilNextRefresh.push_back(0);
        currentWidgetsGroup = {};
    }

};

#endif