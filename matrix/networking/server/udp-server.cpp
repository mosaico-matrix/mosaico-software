#include "../../logger/logger.h"
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <cstring>

#define UDP_PORT 9182
#define BUFFER_SIZE 1024

class UdpServer {
private:
    int server_fd;
    int addrlen = 0;

public:
    void start() {

        Logger::logInfo("Starting UDP server...");

        struct sockaddr_in server_addr;
        int opt = 1;
        addrlen = sizeof(server_addr);

        // Creating socket file descriptor
        if ((server_fd = socket(AF_INET, SOCK_DGRAM, 0)) == 0) {
            Logger::logError("socket failed");
            return;
        }

        // Forcefully attaching socket to the port
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
            Logger::logError("setsockopt failed");
            return;
        }

        // Initialize server information
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = INADDR_ANY;
        server_addr.sin_port = htons(UDP_PORT);

        // Bind the socket with the server address
        if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr))<0) {
            Logger::logError("bind failed");
            return;
        }
    }

    char getNextInput()
    {
        int valread;
        char buffer[BUFFER_SIZE] = {0};
        struct sockaddr_in client_addr;

        // Receive data from client
        if ((valread = recvfrom(server_fd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, (socklen_t*)&addrlen)) < 0) {
            Logger::logError("recvfrom failed");
            return '0';
        }
        buffer[valread] = '\0';

        // Display received data
        std::cout << "Received: " << buffer << std::endl;
        return buffer[0];
    }

};


