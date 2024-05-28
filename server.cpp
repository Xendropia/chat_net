#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <unordered_map>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include "chat.h" // Include the chat.h header file

#define PORT 8080 

std::unordered_map<std::string, int> clientSockets; // Store client usernames and socket descriptors

void* handleClient(void* arg) {
    int clientSocket = *((int*)arg);
    char buffer[1024] = { 0 };
    std::string clientUsername;

    // Receive username from client
    recv(clientSocket, buffer, 1024, 0);
    clientUsername = buffer;
    clientSockets[clientUsername] = clientSocket;

    std::cout << clientUsername << " connected." << std::endl;

    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytesRead = recv(clientSocket, buffer, 1024, 0);
        if (bytesRead <= 0) {
            std::cout << clientUsername << " disconnected." << std::endl;
            clientSockets.erase(clientUsername);
            close(clientSocket);
            pthread_exit(NULL);
        }

        std::string message = buffer;
        std::cout << "Received: " << message << std::endl;

        // Broadcast message to all connected clients (including sender)
        for (auto& pair : clientSockets) {
            send(pair.second, message.c_str(), message.size(), 0);
        }
    }

    return NULL;
}

int main() {
    int serverSocket, newSocket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == 0) {
        std::cerr << "Socket creation failed." << std::endl;
        return 1;
    }

    // Forcefully attach socket to the port
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        std::cerr << "Setsockopt failed." << std::endl;
        return 1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind
    if (bind(serverSocket, (struct sockaddr*)&address, sizeof(address)) < 0) {
        std::cerr << "Bind failed." << std::endl;
        return 1;
    }

    // Listen
    if (listen(serverSocket, 3) < 0) {
        std::cerr << "Listen failed." << std::endl;
        return 1;
    }

    std::cout << "Server started on port " << PORT << std::endl;

    while (true) {
        newSocket = accept(serverSocket, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        if (newSocket < 0) {
            std::cerr << "Accept failed." << std::endl;
            return 1;
        }

        // Create a new thread for each client
        pthread_t threadId;
        if (pthread_create(&threadId, NULL, handleClient, (void*)&newSocket) != 0) {
            std::cerr << "Thread creation failed." << std::endl;
            return 1;
        }
    }

    return 0;
}