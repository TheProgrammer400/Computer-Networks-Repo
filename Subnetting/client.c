#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024
#define PORT 7000

int main(){
    int sockFd;
    struct sockaddr_in serverAddr;
    socklen_t addrLen = sizeof(serverAddr);

    sockFd = socket(AF_INET, SOCK_STREAM, 0);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);

    connect(sockFd, (struct sockaddr *)&serverAddr, addrLen);

    printf("Connected to the server :- \n");
    printf("Enter the IP Address : ");

    char ip[100];
    scanf("%s", ip);
    send(sockFd, ip, strlen(ip), 0);

    int num;
    printf("Enter the Number of Subnets : ");
    scanf("%d", &num);
    send(sockFd, &num, sizeof(int), 0);

    for (int i = 0; i < num; i++){
        printf("Enter the Size of Subnet-%d : ", i + 1);
        
        int a;
        scanf("%d", &a);
        send(sockFd, &a, sizeof(int), 0);
    }
    
    close(sockFd);
    return 0;
}