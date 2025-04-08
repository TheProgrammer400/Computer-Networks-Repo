#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main(){
    int sockFd = 0;
    struct sockaddr_in serverAddr;
    char buffer[1024] = {0};
    int threshold, timeout;

    printf("Enter Initial Threshold: ");
    scanf("%d", &threshold);

    printf("Enter CWND value at which timeout occurs: ");
    scanf("%d", &timeout);

    sockFd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockFd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }

    if (connect(sockFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    sprintf(buffer, "%d %d", threshold, timeout);
    send(sockFd, buffer, strlen(buffer), 0);

    close(sockFd);
    return 0;
}
