//#ifndef TETRIS_CPP
//#define TETRIS_CPP
//
//#include "game.cpp"
//#include "../drawables/drawable-shape.cpp"
//
//#define TETROMINO_START_FALL_SPEED 400
//#define BOARD_START_X 5
//#define BOARD_START_Y 1
//#define BOARD_WIDTH 30
//#define BOARD_HEIGHT 62
//
//// Define some custom shapes for the game
//class Tetromino : public DrawableShape {
//public:
//    int shape[12][12];
//    Color color;
//    enum TetrominoType {
//        I,
//        O,
//        T,
//        S,
//        Z,
//        J,
//        L
//    };
//
//    Tetromino() : DrawableShape() {}
//
//    Tetromino(TetrominoType type) : DrawableShape() {
//
//        switch (type) {
//            case I:
//                color = CYAN_COLOR;
//                break;
////            case O:
////               // _createOTetromino();
////                break;
////            case T:
////               // _createTTetromino();
////                break;
////            case S:
////               // _createSTetromino();
////                break;
////            case Z:
////                //_createZTetromino();
////                break;
////            case J:
////               // _createJTetromino();
////                break;
////            case L:
////                //_createLTetromino();
////                break;
//        }
//
//
//        // Set the color
//
//    }
//
//    void _draw(Canvas *canvas) override {
//        for (int i = 0; i < 4; ++i) {
//            for (int j = 0; j < 4; ++j) {
//                if (shape[i][j] == 1) {
//                    canvas->SetPixel(xPosition + i, yPosition + j, color.r, color.g, color.b);
//                }
//            }
//        }
//    }
//};
//
//class TetrisGameFrame : public DrawableShape {
//public:
//    TetrisGameFrame() : DrawableShape() {}
//
//    void _draw(Canvas *canvas) override {
//
//        // 32x64 board
//        for (int i = 0; i < 32; i++) {
//            for (int j = 0; j < 64; j++) {
//                if (i == 0 || i == 31 || j == 0 || j == 63) {
//                    canvas->SetPixel(xPosition + i, yPosition + j, color.r, color.g, color.b);
//                }
//            }
//        }
//    }
//
//};
//
//class Tetris : public Game {
//
//private:
//
//    int getTetrominoStartX() {
//        return BOARD_START_X + BOARD_WIDTH / 2 - 2;
//    }
//
//public:
//    std::vector<Tetromino*> tetrominos;
//    Tetromino* currentTetromino;
//    Tetromino* nextTetromino;
//
//    Tetris() : Game() {
//
//        // Create board
//        TetrisGameFrame* gameFrame = new TetrisGameFrame();
//        registerDrawable(gameFrame);
//        gameFrame->moveTo(BOARD_START_X-1, BOARD_START_Y-1);
//
//        // Create the first tetromino
//        currentTetromino = new Tetromino(Tetromino::I);
//        registerDrawable(currentTetromino);
//        currentTetromino->moveTo(getTetrominoStartX(), BOARD_START_Y);
//    }
//
//    void renderNextCanvasLayer(CanvasLayer *canvas) override {
//
//
//        // Move the current tetromino down
//        currentTetromino->animateTo(currentTetromino->getX(), currentTetromino->getY() + 1, TETROMINO_START_FALL_SPEED);
//    }
//};
//
//#endif
