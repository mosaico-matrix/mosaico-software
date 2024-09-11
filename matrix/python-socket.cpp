
#include "../../../logger/logger.h"
#include <cstring>
#include <signal.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sstream>
#include "../../../configs.cpp"
#include "../../../external/json/json.hpp"
#include "networking/socket.cpp"
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
        std::string pythonScriptPath = Configs::getPythonScriptPath();
        std::string command = ". "
                              + pythonScriptPath
                              + "/venv/bin/activate && "
                              + pythonScriptPath
                              + "/venv/bin/python3 "
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

public:
    int server_fd{}, new_socket{}, valread{};
    struct sockaddr_in address{};
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    PythonSocket() {
        startPythonServer();

        if (createSocket(server_fd) != 0) {
            Logger::logFatal("Failed to create socket");
        }

        if (setSocketOptions(server_fd, opt) != 0) {
            Logger::logFatal("Failed to set socket options");
        }

        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(10000);

        if (bindSocket(server_fd, address) != 0) {
            Logger::logFatal("Failed to bind socket");
        }

        if (startListening(server_fd) != 0) {
            Logger::logFatal("Failed to start listening");
        }
    }

    std::pair<std::string, json> waitNextCommand() {
        if (acceptConnection(server_fd, new_socket, address, addrlen) != 0) {
            Logger::logFatal("Failed to accept connection");
        }

        std::memset(buffer, 0, sizeof(buffer));
        if (readSocket(new_socket, buffer, sizeof(buffer), valread) != 0) {
            Logger::logFatal("Failed to read from socket");
        }

        std::string receivedData(buffer, valread);  // Only use the valid part of the buffer
        if (!receivedData.empty()) {
            size_t spacePos = receivedData.find(' ');
            if (spacePos != std::string::npos) {
                std::string command = receivedData.substr(0, spacePos);
                std::string data = receivedData.substr(spacePos + 1);

                Logger::logDebug("Received command: " + command + " with data: " + data);

                try {
                    return {std::move(command), json::parse(data)};
                } catch (const json::parse_error &e) {
                    Logger::logError("JSON parse error: " + std::string(e.what()));
                    return {std::move(command), json()};
                }
            } else {
                Logger::logDebug("Received command: " + receivedData + " with no data");
                return {std::move(receivedData), json()};
            }
        }

        return {"", ""};
    }

    void sendResponse(const json &response = json()) const {
        std::string responseStr = response.dump();
        send(new_socket, responseStr.c_str(), responseStr.length(), 0);
    }

    ~PythonSocket() {
        closeSocket(server_fd);

        if (python_pid <= 0)
            return;

        Logger::logInfo("Killing Python server with PID: " + std::to_string(python_pid));
        kill(python_pid, SIGKILL);
    }
};
