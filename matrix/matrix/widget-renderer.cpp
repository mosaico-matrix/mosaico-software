#ifndef MATRIX_SLIDESHOW_CPP
#define MATRIX_SLIDESHOW_CPP

#include <unistd.h>
#include <list>
#include "widgets/matrix-widget.h"
#include "../networking/client/api-client.cpp"
#include "widgets/loading-widget.cpp"
#include "widgets/idle-widget.cpp"

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

    //std::vector<MatrixWidget*> currentWidgetsGroup;   // The current widgets that are displayed THIS IS LEGACY, UNCOMMENT IF NEED TO RENDER MORE THAN ONE WIDGET AT A TIME
    MatrixWidget *currentWidget = nullptr;              // The current widget that is displayed
    std::vector<int> framesUntilNextRefresh;            // The frames until the next refresh for each widget [0,5,2] means the first widget will be refreshed in 0 frames, the second in 5 and the third in 2
    MatrixDevice *matrix;                               // The matrix
    CanvasBuffer *compositeCanvasBuffer;                // Buffer with frames to prevent flickering

public:

    explicit WidgetRenderer(MatrixDevice *matrix) : matrix(matrix) {
        compositeCanvasBuffer = new CanvasBuffer(matrix, CANVAS_BUFFER_SIZE);
    }

    // Destructor
    ~WidgetRenderer() {
        delete compositeCanvasBuffer;
        delete currentWidget;
//        for (auto widget: currentWidgetsGroup) {
//            delete widget;
//        }
    }

    void renderOnMatrix() {

        // Early exit if currentWidget is not set
        if (currentWidget == nullptr) {
            Logger::logError("currentWidget is not initialized");
            return;
        }

        // Record start time
        auto startTime = std::chrono::steady_clock::now();

        // Render the current widget on the canvas
        CanvasLayer *partialLayer = currentWidget->renderCanvasLayer();

        // Check how many pixels in this frame
        Logger::logError("Partial layer has " + std::to_string(partialLayer->getPixelCount()) + " pixels");
        compositeCanvasBuffer->paintPartialLayerOnCurrentFrame(partialLayer);

        // Swap the canvas to the matrix
        compositeCanvasBuffer->loadNextFrameOnMatrix();

        // Record end time
        auto endTime = std::chrono::steady_clock::now();

        // Calculate elapsed time
        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

        // Get the widget's requested frame rate (fps)
        int fps = currentWidget->getFrameRate();
        if(fps > MATRIX_REFRESH_RATE) {
            Logger::logError("Widget requested frame rate is higher than the matrix refresh rate, falling back to default");
            fps = MATRIX_REFRESH_RATE;
        }

        // Calculate sleep time based on the requested fps
        if (fps > 0) {
            int sleepTime = 1000 / fps - elapsedTime;
            if (sleepTime > 0) {
                std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
            }
        }
    }



    void setDynamicWidget(const string &widgetDirPath, const string &configDirPath) {
        // framesUntilNextRefresh.push_back(0); // TODO: this should be adjusted when using more than one widget
        // currentWidgetsGroup = {new DynamicWidget(widgetDirPath, configDirPath)};
        currentWidget = new DynamicWidget(widgetDirPath, configDirPath);
    }

    void setLoading() {
        currentWidget = new LoadingWidget();
    }

    void setIdle() {
        currentWidget = new IdleWidget();
        // framesUntilNextRefresh.push_back(0);
        //  currentWidgetsGroup = {};
    }

};

#endif

//void renderOnMatrix()
//{
//    // Record start time
//    auto startTime = std::chrono::steady_clock::now();
//
//    // Check which widgets need to be refreshed
//    for (int widgetIndex = 0; widgetIndex < currentWidgetsGroup.size(); widgetIndex++)
//    {
//        // Check if the runner needs to be refreshed
//        if (framesUntilNextRefresh[widgetIndex] == 0)
//        {
//            // Render the runner on the canvas
//            CanvasLayer *partialLayer = currentWidgetsGroup[widgetIndex]->renderCanvasLayer();
//            compositeCanvasBuffer->paintPartialLayerOnCurrentFrame(partialLayer);
//
//            // Reset the frames until the next refresh
//            if (currentWidgetsGroup[widgetIndex]->getFrameRate() > 0)
//                framesUntilNextRefresh[widgetIndex] = MATRIX_REFRESH_RATE / currentWidgetsGroup[widgetIndex]->getFrameRate() - 1;
//            else
//                // If the frame rate is 0, render only once (set to -1 to skip the refresh)
//                framesUntilNextRefresh[widgetIndex] = -1;
//        }
//        else if (framesUntilNextRefresh[widgetIndex] > 0)
//        {
//            // Decrement the frames until the next refresh
//            framesUntilNextRefresh[widgetIndex]--;
//
//            // Use last rendered frame
//            compositeCanvasBuffer->paintPartialLayerOnCurrentFrame(currentWidgetsGroup[widgetIndex]->getLastRenderedFrame());
//        }
//    }
//
//    // Swap the canvas to the matrix
//    compositeCanvasBuffer->loadNextFrameOnMatrix();
//
//    if(render < 40) {
//        render++;
//        return;
//    }
//
//    // Record end time
//    auto endTime = std::chrono::steady_clock::now();
//
//    // Calculate elapsed time
//    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
//
//    // Calculate sleep time based on the target frame rate
//    int sleepTime = 1000 / MATRIX_REFRESH_RATE - elapsedTime;
//    if (sleepTime > 0)
//        std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
//}