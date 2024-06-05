#ifndef GAME_CPP
#define GAME_CPP

#include "../matrix-runner.h"

class Game : public MatrixRunner {

public:
    Game(CanvasLayerPosition canvasLayerPosition = CanvasLayerPosition::FULL) : MatrixRunner(MATRIX_REFRESH_RATE, canvasLayerPosition) {
    }

protected:
    char getInput() {
        return ' ';
    }
private:
};
#endif