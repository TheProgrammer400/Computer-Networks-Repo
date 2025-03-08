#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main() {
    int serverFd, clientFd;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientLen = sizeof(clientAddr);
    char buffer[BUFFER_SIZE];

    // Create socket
    serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverFd == -1) error("Socket failed");

    // Bind address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if (bind(serverFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
        error("Bind failed");

    // Listen for connections
    if (listen(serverFd, 5) == -1) error("Listen failed");

    printf("Server is listening on port %d...\n", PORT);

    // Accept client
    clientFd = accept(serverFd, (struct sockaddr*)&clientAddr, &clientLen);
    if (clientFd == -1) error("Accept failed");

    // Receive and echo message
    int bytesRead = read(clientFd, buffer, BUFFER_SIZE);
    if (bytesRead > 0) {
        buffer[bytesRead] = '\0';
        printf("Received: %s", buffer);
        write(clientFd, buffer, bytesRead);
    }

    // Cleanup
    close(clientFd);
    close(serverFd);
    return 0;
}
