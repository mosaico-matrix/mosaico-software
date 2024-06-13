//
// Created by Marco Coppola (and ChatGPT lol)
// This is actually a simple socket listener for the python scripts that actually handle the BLE communication.
//

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "../../../logger/logger.h"
#include <cstring>
#include <signal.h>
#include <sstream>
#include "../../../configs.cpp"

class BleServer {
private:

    pid_t python_pid = 0;

    void startBleServer() {

        string pythonScriptPath = Configs::getPythonScriptPath();
        string command = "source " + pythonScriptPath + "/venv/bin/activate && python3 " + pythonScriptPath + "/main.py & echo $!";
        FILE *pipe = popen(command.c_str(), "r");
        if (!pipe) {
            Logger::logFatal("Failed to start Python BLE server");
        }

        char buffer[128];
        std::memset(buffer, 0, sizeof(buffer));
        if (fgets(buffer, 128, pipe) != nullptr) {
            python_pid = atoi(buffer);
            Logger::logInfo("Python BLE server started with PID: " + std::to_string(python_pid));
        }
        else {
            Logger::logFatal("Failed to start Python BLE server");
        }

        pclose(pipe);
    }

    int createSocket(int &server_fd) {
        // Creating socket file descriptor
        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
            perror("socket failed");
            return -1;
        }
        return 0;
    }

    int setSocketOptions(int server_fd, int &opt) {
        // Forcefully attaching socket to the port 10000
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                       &opt, sizeof(opt))) {
            perror("setsockopt");
            return -1;
        }
        return 0;
    }


    int bindSocket(int server_fd, struct sockaddr_in &address) {
        // Binding socket to the port 10000
        if (bind(server_fd, (struct sockaddr *) &address, sizeof(address)) < 0) {
            perror("bind failed");
            return -1;
        }
        return 0;
    }

    int startListening(int server_fd) {
        // Start listening for connections
        if (listen(server_fd, 3) < 0) {
            perror("listen");
            return -1;
        }
        return 0;
    }

    int acceptConnection(int server_fd, int &new_socket, struct sockaddr_in &address, int &addrlen) {
        // Accept connection
        if ((new_socket = accept(server_fd, (struct sockaddr *) &address, (socklen_t *) &addrlen)) < 0) {
            perror("accept");
            return -1;
        }
        return 0;
    }

    int readSocket(int new_socket, char *buffer, int bufferSize, int &valread) {
        // Read data from socket
        valread = read(new_socket, buffer, bufferSize);
        if (valread < 0) {
            perror("read");
            return -1;
        }
        return 0;
    }

    void closeSocket(int server_fd) {
        close(server_fd);
    }

public:

    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    BleServer() {

        // Start python server in background
        startBleServer();

        // Create socket
        if (createSocket(server_fd) != 0) {
            Logger::logFatal("Failed to create socket");
        }

        // Set socket options
        if (setSocketOptions(server_fd, opt) != 0) {
            Logger::logFatal("Failed to set socket options");
        }

        // Set up the address structure
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(10000);

        // Bind socket
        if (bindSocket(server_fd, address) != 0) {
            Logger::logFatal("Failed to bind socket");
        }

        // Start listening
        if (startListening(server_fd) != 0) {
            Logger::logFatal("Failed to start listening");
        }
    }


    std::pair<std::string, std::string> waitNextCommand() {

        // Accept connection
        if (acceptConnection(server_fd, new_socket, address, addrlen) != 0) {
            Logger::logFatal("Failed to accept connection");
        }

        // Read from socket
        if (readSocket(new_socket, buffer, sizeof(buffer), valread) != 0) {
            Logger::logFatal("Failed to read from socket");
        }

        // Return command and data
        std::string receivedData(buffer);
        if (!receivedData.empty()) {

            // Parse command and data
            std::istringstream iss(receivedData);
            std::string command;
            std::string data;
            iss >> command >> data;

            return {std::move(command), std::move(data)};
        }

        return {"", ""};
    }


    void sendResponse(const char *response) {
        // Send response back to Python
        send(new_socket, response, strlen(response), 0);
    }

    ~BleServer() {
        // Close socket
        closeSocket(server_fd);

        if (python_pid <= 0)
            return;

        // Kill python server
        Logger::logInfo("Killing Python BLE server with PID: " + std::to_string(python_pid));
        kill(python_pid, SIGKILL); // or SIGKILL if you want to force kill
    }


};

