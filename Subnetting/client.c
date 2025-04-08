
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024
#define PORT 7000

int main(){
    int sock;
    struct sockaddr_in address;
    socklen_t addr_len = sizeof(address);

    sock = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);

    connect(sock, (struct sockaddr *)&address, addr_len);

    printf("Connected to the server :- \n");
    printf("Enter the IP Address : ");

    char ip[100];
    scanf("%s", ip);
    send(sock, ip, strlen(ip), 0);

    int num;
    printf("Enter the Number of Subnets : ");
    scanf("%d", &num);
    send(sock, &num, sizeof(int), 0);

    for (int i = 0; i < num; i++){
        printf("Enter the Size of Subnet-%d : ", i + 1);
        
        int a;
        scanf("%d", &a);
        send(sock, &a, sizeof(int), 0);
    }
    
    close(sock);
    return 0;
}