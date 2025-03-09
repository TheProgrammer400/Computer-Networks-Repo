#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int serverFd, clientFd;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrSize = sizeof(clientAddr);
    char buffer[BUFFER_SIZE];

    // Create socket
    serverFd = socket(AF_INET, SOCK_STREAM, 0);
    
    // Bind to port
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);
    bind(serverFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

    // Listen for connections
    listen(serverFd, 5);
    printf("Server is listening on port %d...\n", PORT);

    // Accept client connection
    clientFd = accept(serverFd, (struct sockaddr*)&clientAddr, &addrSize);
    printf("Client connected!\n");

    // Chat loop
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        recv(clientFd, buffer, BUFFER_SIZE, 0);
        printf("Client: %s", buffer);
        
        if (strncmp(buffer, "exit", 4) == 0) {
            printf("Chat ended by client.\n");
            break;
        }

        printf("You: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        send(clientFd, buffer, strlen(buffer), 0);

        if (strncmp(buffer, "exit", 4) == 0) {
            printf("Chat ended by server.\n");
            break;
        }
    }

    close(clientFd);
    close(serverFd);
    return 0;
}
