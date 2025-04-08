#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>

#define PORT 7000
#define BUFFER_SIZE 1024

void validateIP(char *ip, int arr[]){
    int defaultMask = 0;
    char class;
    char *a = ".";
    char *token = strtok(ip, a);
    int flag = 1;
    int index = 0;

    while (token != NULL){
        int num = atoi(token);

        if (num <= 255 && num >= 0){
            if (num < 128){
                class = 'A';
                defaultMask = 8;
            } else if (num > 127 && num <= 191){
                class = 'B';
                defaultMask = 16;
            } else if (num > 191 && num <= 223){
                class = 'C';
                defaultMask = 24;
            } else if (num > 223 && num <= 239){
                class = 'D';
            } else {
                class = 'E';
            }

            arr[index++] = num;
        } else {
            flag = 0;
        }

        token = strtok(NULL, a);
    }

    if (flag == 0) {
        printf("Invalid IP\n");
        exit(1);
    } else {
        printf("The given IP is Correct\n");
        printf("Class of the Given IP : %c\n", class);
        printf("Default Network Mask : %d\n", defaultMask);
    }
}

int power_of_2(int n) {
    if (n == 0) {
        return 1;
    }

    n--;

    n = n | n >> 1;
    n = n | n >> 2;
    n = n | n >> 4;
    n = n | n >> 8;
    n = n | n >> 16;

    return n + 1;
}

int power(int a){
    int p = 0;

    while (a > 1){
        a >>= 1;
        p++;
    }

    return p;
}

int main(){
    int numSubnets;
    int ip[4];
    char buffer[BUFFER_SIZE];
    int serverFd, clientFd;
    struct sockaddr_in serverAddr;
    int subnetMask;
    socklen_t addrLen = sizeof(serverAddr);

    serverFd = socket(AF_INET, SOCK_STREAM, 0);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(PORT);

    bind(serverFd, (struct sockaddr *)&serverAddr, addrLen);
    listen(serverFd, 10);
    printf("Server is Listening to Port %d\n", PORT);

    clientFd = accept(serverFd, (struct sockaddr *)&serverAddr, &addrLen);
    printf("Connected to the Client\n");

    int bytes = recv(clientFd, buffer, BUFFER_SIZE, 0);
    validateIP(buffer, ip);

    bytes = recv(clientFd, &numSubnets, sizeof(numSubnets), 0);
    printf("Number of Subnets = %d\n", numSubnets);

    int size[numSubnets];
    printf("Subnet Sizes: ");

    for (int i = 0; i < numSubnets; i++){
        bytes = recv(clientFd, &size[i], sizeof(int), 0); // Correct size
        printf("%d ", size[i]);
    }

    printf("\n");

    printf("Number of Addresses Alloted to each Subnet: ");

    for (int i = 0; i < numSubnets; i++){
        size[i] = power_of_2(size[i]);
        printf("%d ", size[i]);
    }

    printf("\n");

    int currentIP[4] = {ip[0], ip[1], ip[2], ip[3]};

    for (int i = 0; i < numSubnets; i++){
        subnetMask = 32 - power(size[i]);

        int subnet_start = currentIP[3];
        int subnet_end = currentIP[3] + size[i] - 1;

        // Adjust the subnet end if it exceeds 255
        while (subnet_end > 255){
            int overflow = subnet_end - 255;

            currentIP[3] = 255;
            currentIP[2]++;
            subnet_end = overflow - 1;
        }

        // Print the IP range for the subnet
        printf("IP Range for Subnet-%d: %d.%d.%d.%d - %d.%d.%d.%d\n",
               i + 1, currentIP[0], currentIP[1], currentIP[2], subnet_start,
               currentIP[0], currentIP[1], currentIP[2], subnet_end);

        // Update the current_ip for the next subnet
        currentIP[3] = subnet_end + 1;

        // Handle overflow in octets
        while (currentIP[3] > 255){
            currentIP[3] -= 256;
            currentIP[2]++;
        }

        // Handle overflow in second octet
        while (currentIP[2] > 255){
            currentIP[2] -= 256;
            currentIP[1]++;
        }

        // Handle overflow in first octet
        while (currentIP[1] > 255){
            currentIP[1] -= 256;
            currentIP[0]++;
        }
    }

    close(serverFd);
    close(clientFd);
    return 0;
}
