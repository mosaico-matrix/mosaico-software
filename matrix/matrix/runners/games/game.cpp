#ifndef GAME_CPP
#define GAME_CPP

#include "../matrix-runner.h"
#include "../../../networking/server/udp-server.cpp"

class Game : public MatrixRunner {

public:
    Game(CanvasLayerPosition canvasLayerPosition = CanvasLayerPosition::FULL) : MatrixRunner(MATRIX_REFRESH_RATE, canvasLayerPosition) {
        // Open UDP server to receive input
        udpServer = new UdpServer();
        udpServer->start();
    }

protected:
    char getInput() {
        return udpServer->getNextInput();
    }
private:
    UdpServer *udpServer;
};
#endif