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
    int sockFd;
    struct sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE];

    // Create socket
    sockFd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockFd == -1) {
        error("Socket failed");
    }

    // Set server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to server
    if (connect(sockFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1){
        error("Connect failed");
    }

    // Send message
    printf("Enter message: ");
    fgets(buffer, BUFFER_SIZE, stdin);
    write(sockFd, buffer, strlen(buffer));

    // Receive echo
    int bytesRead = read(sockFd, buffer, BUFFER_SIZE);
    
    if (bytesRead > 0) {
        buffer[bytesRead] = '\0';
        printf("Echoed: %s", buffer);
    }

    // Cleanup
    close(sockFd);
    return 0;
}
