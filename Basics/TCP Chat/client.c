#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int clientFd;
    struct sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE];

    // Create socket
    clientFd = socket(AF_INET, SOCK_STREAM, 0);

    // Define server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to server
    connect(clientFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    printf("Connected to server!\n");

    // Chat loop
    while (1) {
        printf("You: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        send(clientFd, buffer, strlen(buffer), 0);

        if (strncmp(buffer, "exit", 4) == 0) {
            printf("Chat ended by client.\n");
            break;
        }

        memset(buffer, 0, BUFFER_SIZE);
        recv(clientFd, buffer, BUFFER_SIZE, 0);
        printf("Server: %s", buffer);

        if (strncmp(buffer, "exit", 4) == 0) {
            printf("Chat ended by server.\n");
            break;
        }
    }

    close(clientFd);
    return 0;
}