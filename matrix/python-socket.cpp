//
// Created by Marco Coppola (and ChatGPT lol)
// This is actually a simple socket listener for the python scripts that actually handle the BLE communication.
// It's a simple socket listener that listens for commands from the Python scripts and sends responses back.
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "../../../logger/logger.h"
#include <cstring>
#include <signal.h>
#include <sstream>
#include "../../../configs.cpp"
#include "../../../external/json/json.hpp"

using json = nlohmann::json;

#define BUFFER_SIZE 2048

class PythonSocket {
private:

    pid_t python_pid = 0;
    std::string pythonScriptArguments =
#if SIMULATION
            "simulation";
#elif WEB
            "web";
#else
            "";
#endif

    void startPythonServer() {

        string pythonScriptPath = Configs::getPythonScriptPath();
        string command = ". "
                         + pythonScriptPath
                         + "/venv/bin/activate && python3 "
                         + pythonScriptPath
                         + "/main.py "
                         + pythonScriptArguments
                         + " & echo $!";
        Logger::logInfo("Starting Python server with command: " + command);
        FILE *pipe = popen(command.c_str(), "r");
        if (!pipe) {
            Logger::logFatal("Failed to start Python server");
        }

        char buffer[BUFFER_SIZE];
        std::memset(buffer, 0, sizeof(buffer));
        if (fgets(buffer, 128, pipe) != nullptr) {
            python_pid = atoi(buffer);
            Logger::logInfo("Python server started with PID: " + std::to_string(python_pid));
        } else {
            Logger::logFatal("Failed to start Python server");
        }

        pclose(pipe);
    }

    static int createSocket(int &server_fd) {
        // Creating socket file descriptor
        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
            perror("socket failed");
            return -1;
        }
        return 0;
    }

    static int setSocketOptions(int server_fd, int &opt) {
        // Forcefully attaching socket to the port 10000
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                       &opt, sizeof(opt))) {
            perror("setsockopt");
            return -1;
        }
        return 0;
    }


    static int bindSocket(int server_fd, struct sockaddr_in &address) {
        // Binding socket to the port 10000
        if (bind(server_fd, (struct sockaddr *) &address, sizeof(address)) < 0) {
            perror("bind failed");
            return -1;
        }
        return 0;
    }

    static int startListening(int server_fd) {
        // Start listening for connections
        if (listen(server_fd, 3) < 0) {
            perror("listen");
            return -1;
        }
        return 0;
    }

    static int acceptConnection(int server_fd, int &new_socket, struct sockaddr_in &address, int &addrlen) {
        // Accept connection
        if ((new_socket = accept(server_fd, (struct sockaddr *) &address, (socklen_t * ) & addrlen)) < 0) {
            perror("accept");
            return -1;
        }
        return 0;
    }

    static int readSocket(int new_socket, char *buffer, int bufferSize, int &valread) {
        // Read data from socket
        valread = read(new_socket, buffer, bufferSize);
        if (valread < 0) {
            perror("read");
            return -1;
        }
        return 0;
    }

    static void closeSocket(int server_fd) {
        close(server_fd);
    }

public:

    int server_fd{}, new_socket{}, valread{};
    struct sockaddr_in address{};
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    PythonSocket() {

        // Start python server in background
        startPythonServer();

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


    std::pair <std::string, json> waitNextCommand() {
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
            // Find the first space
            size_t spacePos = receivedData.find(' ');
            if (spacePos != std::string::npos) {
                std::string command = receivedData.substr(0, spacePos);
                std::string data = receivedData.substr(spacePos + 1);

                Logger::logDebug("Received command: " + command + " with data: " + data);

                return {std::move(command), json::parse(data)};
            } else {
                // Handle the case where there's no space, assuming the whole receivedData is the command
                Logger::logDebug("Received command: " + receivedData + " with no data");

                return {std::move(receivedData), json()};
            }
        }

        return {"", ""};
    }


    void sendResponse(const json &response = json()) const {

        // Convert response to string
        std::string responseStr = response.dump();

        // Send response back to Python
        send(new_socket, responseStr.c_str(), responseStr.length(), 0);
    }

    ~PythonSocket() {
        // Close socket
        closeSocket(server_fd);

        if (python_pid <= 0)
            return;

        // Kill python server
        Logger::logInfo("Killing Python server with PID: " + std::to_string(python_pid));
        kill(python_pid, SIGKILL); // or SIGKILL if you want to force kill
    }


};

