#pragma once
#include <cstring>
#include <signal.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

static int createSocket(int &server_fd) {
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        return -1;
    }
    return 0;
}

static int setSocketOptions(int server_fd, int &opt) {
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt))) {
        perror("setsockopt");
        return -1;
    }
    return 0;
}

static int bindSocket(int server_fd, struct sockaddr_in &address) {
    if (bind(server_fd, (struct sockaddr *) &address, sizeof(address)) < 0) {
        perror("bind failed");
        return -1;
    }
    return 0;
}

static int startListening(int server_fd) {
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        return -1;
    }
    return 0;
}

static int acceptConnection(int server_fd, int &new_socket, struct sockaddr_in &address, int &addrlen) {
    if ((new_socket = accept(server_fd, (struct sockaddr *) &address, (socklen_t * ) & addrlen)) < 0) {
        perror("accept");
        return -1;
    }
    return 0;
}

static int readSocket(int new_socket, char *buffer, int bufferSize, int &valread) {
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