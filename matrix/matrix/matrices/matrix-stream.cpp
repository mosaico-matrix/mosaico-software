#pragma once

#include "matrix-device.cpp"
#include "../../external/rpi-rgb-led-matrix/include/led-matrix.h"
#include "../../networking/socket.cpp"
#include <cstring>
#include <signal.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "../../configs.cpp"

using namespace rgb_matrix;

#define STREAM_BUFFER_SIZE 2048


class MatrixStreamCanvas : public Canvas {
private:
    std::vector <uint8_t> pixelData;
    static const int PIXEL_SIZE = 3;  // RGB

public:
    MatrixStreamCanvas() {
        pixelData.resize(MATRIX_COLS * MATRIX_ROWS * PIXEL_SIZE, 0);
    }

    ~MatrixStreamCanvas() {}

    void SetPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue) override {
        int index = (y * MATRIX_COLS + x) * PIXEL_SIZE;
        pixelData[index] = red;
        pixelData[index + 1] = green;
        pixelData[index + 2] = blue;
    }

    void Fill(uint8_t red, uint8_t green, uint8_t blue) override {
        for (size_t i = 0; i < pixelData.size(); i += PIXEL_SIZE) {
            pixelData[i] = red;
            pixelData[i + 1] = green;
            pixelData[i + 2] = blue;
        }
    }

    void Clear() override {
        std::fill(pixelData.begin(), pixelData.end(), 0);
    }

    int width() const override {
        return MATRIX_COLS;
    }

    int height() const override {
        return MATRIX_ROWS;
    }

    const std::vector <uint8_t> &getPixelData() const {
        return pixelData;
    }
};

class MatrixStream : public MatrixDevice {
private:
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    static const int PORT = 10005;
    pid_t python_pid = 0;

public:
    MatrixStream() {
        if (createSocket(server_fd) != 0) {
            Logger::logFatal("Failed to create socket");
        }

        if (setSocketOptions(server_fd, opt) != 0) {
            Logger::logFatal("Failed to set socket options");
        }

        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(PORT);

        if (bindSocket(server_fd, address) != 0) {
            Logger::logFatal("Failed to bind socket");
        }

        if (startListening(server_fd) != 0) {
            Logger::logFatal("Failed to start listening");
        }

        Logger::logInfo("Waiting for connection on port " + std::to_string(PORT) + " for MatrixStream");

        // Start python server
        std::string pythonStreamPath = Configs::getPythonStreamPath();
        std::string command = ". "
                              + pythonStreamPath
                              + "/venv/bin/activate && "
                              + pythonStreamPath
                              + "/venv/bin/python3 "
                              + pythonStreamPath
                              + "/stream.py "
                              + " & echo $!";

        Logger::logInfo("Starting stream server with command: " + command);
        FILE *pipe = popen(command.c_str(), "r");
        if (!pipe) {
            Logger::logFatal("Failed to start stream server");
        }

        if (acceptConnection(server_fd, new_socket, address, addrlen) != 0) {
            Logger::logFatal("Failed to accept connection");
        }


        char buffer[STREAM_BUFFER_SIZE];
        std::memset(buffer, 0, sizeof(buffer));
        if (fgets(buffer, 128, pipe) != nullptr) {
            python_pid = atoi(buffer);
            Logger::logInfo("Python stream server started with PID: " + std::to_string(python_pid));
        } else {
            Logger::logFatal("Failed to start Python stream server");
        }

    }

    ~MatrixStream() {
        closeSocket(server_fd);
        close(new_socket);

        Logger::logInfo("Killing Python stream with PID: " + std::to_string(python_pid));
        kill(python_pid, SIGKILL);
    }

    Canvas *CreateFrameCanvas() override {
        return new MatrixStreamCanvas();
    }

    void SwapFrameCanvas(Canvas *canvas) override {
        MatrixStreamCanvas *streamCanvas = dynamic_cast<MatrixStreamCanvas *>(canvas);
        if (streamCanvas) {
            const std::vector <uint8_t> &pixelData = streamCanvas->getPixelData();
            ssize_t sentBytes = send(new_socket, pixelData.data(), pixelData.size(), 0);
            if (sentBytes < 0) {
                perror("send");
                Logger::logFatal("Pipe was closed");
            } else if (sentBytes != static_cast<ssize_t>(pixelData.size())) {
                Logger::logError("Sent bytes mismatch");
            }
        } else {
            Logger::logError("Canvas is not of type MatrixStreamCanvas");
        }
    }

    void SetPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue) override {
    }

    void Fill(uint8_t red, uint8_t green, uint8_t blue) override {
    }

    void Clear() override {
    }
};
