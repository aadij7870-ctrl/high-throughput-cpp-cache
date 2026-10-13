#include <iostream>
#include <string>
#include <sstream>
#include <thread>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include "ThreadSafeCache.hpp"

ThreadSafeCache globalCache(1000);

void handleClient(int clientSocket) {
    char buffer[1024];
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytesRead = read(clientSocket, buffer, sizeof(buffer) - 1);
        if (bytesRead <= 0) break;

        std::string command(buffer);
        std::stringstream ss(command);
        std::string action, key, value;
        ss >> action >> key;

        std::string response;
        if (action == "SET") {
            ss >> value;
            globalCache.set(key, value);
            response = "OK\n";
        } else if (action == "GET") {
            auto result = globalCache.get(key);
            if (result.has_value()) {
                response = result.value() + "\n";
            } else {
                response = "(nil)\n";
            }
        } else {
            response = "ERROR: Unknown command. Use SET <key> <val> or GET <key>\n";
        }

        write(clientSocket, response.c_str(), response.length());
    }
    close(clientSocket);
}

int main() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8080);

    bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, 10);

    std::cout << "Cache Server listening on port 8080..." << std::endl;

    while (true) {
        int clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket >= 0) {
            std::thread clientThread(handleClient, clientSocket);
            clientThread.detach();
        }
    }

    close(serverSocket);
    return 0;
}
