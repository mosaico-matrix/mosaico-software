#ifndef GAME_CPP
#define GAME_CPP

#include "../matrix-widget.h"

class Game : public MatrixWidget {

public:
    Game(CanvasLayerPosition canvasLayerPosition = CanvasLayerPosition::FULL) : MatrixWidget(MATRIX_REFRESH_RATE, canvasLayerPosition) {
    }

protected:
    char getInput() {
        return ' ';
    }
private:
};
#endif