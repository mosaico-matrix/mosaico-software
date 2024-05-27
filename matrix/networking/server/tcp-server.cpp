//#include <iostream>
//#include <unistd.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <cstring>
//#include <string>
//#include "../../logger/logger.h"
//#include <fcntl.h>
//#include <sys/select.h>
//#include "../../matrix/matrix-runner.cpp"
//#include "../../matrix/matrix-slideshow.cpp"
//#include "../../external/json/json.hpp"
//#include "../../matrix/games/pixel-mover.cpp"
//
//#define TCP_PORT 8080
//#define BUFFER_SIZE 1024
//
//using json = nlohmann::json;
//using namespace std;
//class TcpServer {
//
//private:
//
//    int serverSocket, newSocket, portNum;
//    struct sockaddr_in serverAddr, clientAddr;
//    socklen_t clientLen;
//    std::string okResponse = "OK\n";
//
//
//    /// Send OK response to client
//    void sendOkResponse(int clientSocket) {
//        send(clientSocket, okResponse.c_str(), strlen(okResponse.c_str()), 0);
//    }
//
//    /// Send error response to client
//    void sendErrorResponse(int clientSocket, const string& message) {
//        send(clientSocket, message.c_str(), message.length(), 0);
//    }
//
//    // Handle connection
//    MatrixSlideshow *handleConnection(int clientSocket) {
//
////        char buffer[BUFFER_SIZE] = {0};
////
////        // Receive data from client
////        int valread = read(clientSocket, buffer, BUFFER_SIZE);
////        Logger::logDebug("Received new command: " + string(buffer));
////
////        // Parse JSON
////        json j = json::parse(buffer);
////        string command = j["command"];
////
////        // Status command
////        if (command == "status") {
////            Logger::logDebug("Received status command");
////
////            // Send back matrix status
////            string status = "{\"status\": 1, \"active_slideshow\": 2}\n"; // REMEMBER \n
////            send(clientSocket, status.c_str(), status.length(), 0);
////            return NULL; // Non-blocking command
////        }
////
////        // Start game
////        else if (command == "start_game") {
////
////            Logger::logDebug("Received start_game command");
////
////            // Get game type
////            int game = j["params"]["game_id"];
////
////            // Create new slideshow with game
////            MatrixSlideshow *newSlideshow = new MatrixSlideshow(matrix);
////            newSlideshow->setGame((MatrixGameEnum)game);
////
////            // Send ok to client
////            sendOkResponse(clientSocket);
////            return newSlideshow;
////        }
////
////        // New runner
////        else if (command == "display_widget")
////        {
////            Logger::logDebug("Received display_widget command");
////
////            // Get widget type and id
////            int widgetId = j["params"]["widget_id"];
////            int widgetType = j["params"]["widget_type"];
////
////            // Create new slideshow with widget
////            MatrixSlideshow *newSlideshow = new MatrixSlideshow(matrix);
////            newSlideshow->downloadAndAddWidget(widgetId, (MatrixWidgetEnum)widgetType);
////
////            // Send ok to client
////            sendOkResponse(clientSocket);
////            return newSlideshow;
////        }
////
////        // New slideshow
////        else if (command == "set_slideshow")
////        {
//////            BlockingCommand *tcpCommand = new BlockingCommand();
//////            tcpCommand->runnerId = j["runnerId"];
//////            return tcpCommand;
////        }
////
////        // Send data back to client
////        send(clientSocket, "Server message", strlen("Server message"), 0);
////        close(clientSocket);
//        return NULL;
//    }
//
//
//public:
//    //MatrixProxy *matrix;
//    TcpServer() {
//
//        // Create TCP socket
//        clientLen = sizeof(clientAddr);
//        serverSocket = socket(AF_INET, SOCK_STREAM, 0);
//        if (serverSocket == -1) {
//            Logger::logError("Error creating socket");
//            exit(1);
//            return;
//        }
//
//        // Define server address
//        serverAddr.sin_family = AF_INET;
//        serverAddr.sin_addr.s_addr = INADDR_ANY;
//        serverAddr.sin_port = htons(TCP_PORT);
//
//        // Bind socket to address
//        if (bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0) {
//            Logger::logError("Error binding socket");
//            exit(1);
//            return;
//        }
//
//        // Listen for connections
//        listen(serverSocket, 5);
//    }
//
//    /// Listen for incoming commands on tcp socket
//    /// \return TcpCommand object if blocking command is received
//    /// \return NULL if no command is received or if the command is not blocking
//    MatrixSlideshow *listenForCommands() {
//
//        // Accept incoming connection
//        newSocket = accept(serverSocket, (struct sockaddr *) &clientAddr, &clientLen);
//
//        // Check for errors
//        if (newSocket < 0) {
//            Logger::logError("Error accepting connection");
//            return NULL;
//        }
//
//        Logger::logDebug("Connection accepted");
//
//        // Handle connection in separate function
//        return handleConnection(newSocket);
//    }
//
//
//    void destroySocket() {
//        close(serverSocket);
//    }
//};