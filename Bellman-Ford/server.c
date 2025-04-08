
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>

#define MAX_NODES 100
#define INF 9999

// Bellman-Ford algorithm to calculate the shortest paths
void bellmanFord(int dist[MAX_NODES][MAX_NODES], int nodes, int source, int routingTable[MAX_NODES]) {
    int distCopy[MAX_NODES];
    for (int i = 0; i < nodes; i++) {
        distCopy[i] = dist[source][i];
    }

    for (int k = 0; k < nodes - 1; k++) {
        for (int u = 0; u < nodes; u++) {
            for (int v = 0; v < nodes; v++) {
                if (distCopy[u] != INF && distCopy[u] + dist[u][v] < distCopy[v]) {
                    distCopy[v] = distCopy[u] + dist[u][v];
                }
            }
        }
    }

    // Fill the routing table with the final shortest distances
    for (int i = 0; i < nodes; i++) {
        routingTable[i] = distCopy[i];
    }
}

// Function to create and bind the server socket
int createServerSocket(int port) {
    int serverFd;
    struct sockaddr_in serverAddr;

    serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverFd < 0) {
        perror("Server socket creation failed");
        exit(1);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(serverFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Bind failed");
        exit(1);
    }

    if (listen(serverFd, 3) < 0) {
        perror("Listen failed");
        exit(1);
    }

    return serverFd;
}

int main(){
    int serverFd, clientFd, clientLen;
    struct sockaddr_in clientAddr;
    int nodes;
    int dist[MAX_NODES][MAX_NODES];

    // Create server socket
    serverFd = createServerSocket(8080);

    printf("Server is running. Waiting for connections...\n");

    // Accept incoming connection
    clientLen = sizeof(clientAddr);
    clientFd = accept(serverFd, (struct sockaddr *)&clientAddr, &clientLen);
    
    if (clientFd < 0) {
        perror("Connection failed");
        exit(1);
    }

    // Receive number of nodes
    recv(clientFd, &nodes, sizeof(nodes), 0);
    printf("Received number of nodes: %d\n", nodes);

    // Receive the distance matrix
    recv(clientFd, dist, sizeof(dist), 0);
    printf("Received the distance matrix:\n");

    // Print the received distance matrix (for reference)
    for (int i=0; i<nodes; i++) {
        for (int j=0; j<nodes; j++) {
            if (dist[i][j] == INF)
                printf("INF ");
            else
                printf("%d ", dist[i][j]);
        }
        
        printf("\n");
    }

    // Compute and print the routing tables for each node
    for (int i = 0; i < nodes; i++) {
        int routingTable[MAX_NODES];
        bellmanFord(dist, nodes, i, routingTable);

        // Print the routing table for node i
        printf("\nRouting table for node %d:\n", i);

        for (int j=0; j<nodes; j++) {
            if (routingTable[j] == INF)
                printf("INF ");
            else
                printf("%d ", routingTable[j]);
        }

        printf("\n");
    }

    close(clientFd);
    close(serverFd);
    return 0;
}