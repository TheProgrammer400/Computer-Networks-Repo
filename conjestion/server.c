#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

void handle_congestion_control(int threshold, int timeout) {
    int cwnd = 1;
    printf("Initial CWND: %d, Threshold: %d\n", cwnd, threshold);

    while (cwnd < threshold) {
        cwnd *= 2;
        
        printf("CWND Increased: %d\n", cwnd);
        sleep(1);

        if (cwnd >= timeout) {
            printf("Timeout Occurred at CWND = %d\n", cwnd);
            threshold = cwnd / 2;
            cwnd = 1;
            printf("New Threshold: %d, CWND after timeout: %d\n", threshold, cwnd);
            break;
        }
    }

    while (1) {
        cwnd += 1;
        printf("CWND : %d\n", cwnd);
        sleep(1);
    }
}

int main() {
    int serverFd, clientFd;
    struct sockaddr_in serverAddr;
    int clientLen = sizeof(serverAddr);
    char buffer[1024] = {0};

    serverFd = socket(AF_INET, SOCK_STREAM, 0);

    if (serverFd == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if (bind(serverFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(serverFd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    clientFd = accept(serverFd, (struct sockaddr *)&serverAddr, (socklen_t*)&clientLen);

    if (clientFd < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    read(clientFd, buffer, sizeof(buffer));
    int threshold, timeout;
    sscanf(buffer, "%d %d", &threshold, &timeout);

    printf("Initial Threshold: %d, Timeout at CWND = %d\n", threshold, timeout);
    handle_congestion_control(threshold, timeout);

    close(clientFd);
    close(serverFd);
    return 0;
}