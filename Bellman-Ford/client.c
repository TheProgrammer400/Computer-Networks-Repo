#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>

#define MAX_NODES 100

// Function to input the distance matrix
void inputDistanceMatrix(int nodes, int dist[MAX_NODES][MAX_NODES]) {
    printf("Enter the distance matrix (Use 0 for infinity, The distance from a node to itself is given by default):\n");

    for (int i=0; i<nodes; i++) {
        for (int j=0; j<nodes; j++) {
            if (i == j){
                dist[i][j] = 0;  // distance from a node to itself is 0
            } else {
                printf("Enter distance from node %d to node %d: ", i, j);
                scanf("%d", &dist[i][j]);

                if (dist[i][j] == 0){
                    dist[i][j] = 9999;  // Treat 0 as infinity if not the diagonal
                }
            }
        }

        printf("\n");
    }
}

// Function to create a socket, connect to the server, and send data
int createClientSocket(char *server_ip, int port) {
    int sockFd;
    struct sockaddr_in serverAddr;

    sockFd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockFd < 0) {
        perror("Client socket creation failed");
        exit(1);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr(server_ip);

    if (connect(sockFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Connection failed");
        exit(1);
    }

    return sockFd;
}

int main(){
    int nodes;
    int dist[MAX_NODES][MAX_NODES];
    int sockFd;
    struct sockaddr_in serverAddr;
   
    // Input number of nodes
    printf("Enter the number of nodes: ");
    scanf("%d", &nodes);

    // Input the distance matrix
    inputDistanceMatrix(nodes, dist);

    // Create a socket to communicate with the server
    sockFd = createClientSocket("127.0.0.1", 8080);

    // Send the number of nodes and the distance matrix to the server
    send(sockFd, &nodes, sizeof(nodes), 0);
    send(sockFd, dist, sizeof(dist), 0);

    close(sockFd);
    return 0;
}
