#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>

#define PORT 6000

int main(){
    int sock = 0;
    struct sockaddr_in serv_addr;
    char ip[16];
    int num_subnets;
    int *subnet_sizes;

    if((sock=socket(AF_INET, SOCK_STREAM ,0)) < 0){
        printf("\nSocket creation error\n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0){
        printf("\nInvalid address/Address not supported\n");
        return -1;
    }

    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))<0){
        printf("\nConnection Failed\n");
        return -1;
    }

    printf("Enter IP address (without /prefix): ");
    scanf("%s",ip);

    printf("Enter the number of subnets: ");
    scanf("%d",&num_subnets);

    subnet_sizes=(int*)malloc(num_subnets * sizeof(int));

    printf("Enter size of %d subnets: ",num_subnets);

    for(int i=0; i<num_subnets; i++){
        scanf("%d",&subnet_sizes[i]);
    }

    send(sock, ip, sizeof(ip), 0);
    send(sock, &num_subnets, sizeof(num_subnets),0);
    send(sock, subnet_sizes, num_subnets*sizeof(int), 0);

    free(subnet_sizes);
    close(sock);

    return 0;
}
