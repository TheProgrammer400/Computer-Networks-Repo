#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>

#define PORT 6000

char getIPClass(char *ip, int *network_size) {
    unsigned char first_octet;
    sscanf(ip, "%hhu", &first_octet);

    if (first_octet >= 1 && first_octet <= 126) {
        *network_size = 16777216;
        return 'A';
    } else if (first_octet >= 128 && first_octet <= 191) {
        *network_size = 65536;
        return 'B';
    } else if (first_octet >= 192 && first_octet <= 223) {
        *network_size = 256;
        return 'C';
    }

    return 'X';
}

int isValidIP(char *ip) {
    int octets[4];

    if (sscanf(ip, "%d.%d.%d.%d", &octets[0], &octets[1], &octets[2], &octets[3]) != 4) {
        return 0;

    }
    
    for (int i = 0; i < 4; i++) {
        if (octets[i] < 0 || octets[i] > 255) {
            return 0;
        }
    }

    return 1;
}

void getNetworkAddress(char *ip, struct in_addr *network) {
    unsigned char first_octet, second_octet, third_octet;
    sscanf(ip, "%hhu.%hhu.%hhu", &first_octet, &second_octet, &third_octet);

    if (first_octet >= 1 && first_octet <= 126) {
        *network = (struct in_addr) { htonl((first_octet << 24)) };
    } else if (first_octet >= 128 && first_octet <= 191) {
        *network = (struct in_addr) { htonl((first_octet << 24) | (second_octet << 16)) };
    } else if (first_octet >= 192 && first_octet <= 223) {
        *network = (struct in_addr) { htonl((first_octet << 24) | (second_octet << 16) | (third_octet << 8)) };
    }
}

int nextPowerOf2(int n) {
    int p = 1;

    while (p < n) {
        p *= 2;
    }
    return p;
}

int calculateSubnetMask(int size) {
    int host_bits = 0;
    
    while ((1 << host_bits) < size) {
        host_bits++;
    }
    return 32 - host_bits;
}

void createSubnets(char *ip, int *sizes, int num_subnets, char *response) {
    struct in_addr base_ip;
    int total_network_size;
    char ip_class = getIPClass(ip, &total_network_size);
    getNetworkAddress(ip, &base_ip);

    char net_addr[16];
    inet_ntop(AF_INET, &base_ip, net_addr, sizeof(net_addr));

    sprintf(response + strlen(response), "Valid IP Address: %s\n", ip);
    sprintf(response + strlen(response), "IP Class: %c\n", ip_class);
    sprintf(response + strlen(response), "Network Address: %s\nSubnets:\n", net_addr);

    int assigned_ips = 0;

    for (int i = 0; i < num_subnets; i++) {
        char first_ip[16], last_ip[16];
        int needed_hosts = sizes[i];
        int subnet_size = nextPowerOf2(needed_hosts + 2);
        int mask = calculateSubnetMask(subnet_size);

        struct in_addr first, last;
        first.s_addr = base_ip.s_addr;
        last.s_addr = base_ip.s_addr + htonl(subnet_size - 1);

        inet_ntop(AF_INET, &first, first_ip, sizeof(first_ip));
        inet_ntop(AF_INET, &last, last_ip, sizeof(last_ip));

        sprintf(response + strlen(response), "Subnet %d: %s/%d to %s/%d\n", i + 1, first_ip, mask, last_ip, mask);

        base_ip.s_addr = htonl(ntohl(base_ip.s_addr) + subnet_size);
        assigned_ips += subnet_size;
    }

    int unassigned_ips = total_network_size - assigned_ips;

    sprintf(response + strlen(response), "\nTotal Assigned IPs: %d\n", assigned_ips);
    sprintf(response + strlen(response), "Unassigned IPs: %d\n", unassigned_ips);

    char available_start[16], available_end[16];
    inet_ntop(AF_INET, &base_ip, available_start, sizeof(available_start));

    struct in_addr last_available;

    last_available.s_addr = base_ip.s_addr + htonl(unassigned_ips - 1);
    inet_ntop(AF_INET, &last_available, available_end, sizeof(available_end));

    sprintf(response + strlen(response), "Available IP Range: %s - %s\n", available_start, available_end);
    printf("\nServer Output:\n%s", response);
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char ip[16];
    int num_subnets;
    int *subnet_sizes;
    char response[2048] = { 0 };

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *) &address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server is running on port %d...\n", PORT);

    if ((new_socket = accept(server_fd, (struct sockaddr *) &address, (socklen_t *) &addrlen)) < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    read(new_socket, ip, sizeof(ip));
    printf("Received IP: %s\n", ip);

    if (!isValidIP(ip)) {
        strcpy(response, "Invalid IP address\n");
        printf("Invalid IP address received\n");
        send(new_socket, response, strlen(response), 0);
        close(new_socket);
        close(server_fd);
        return 0;
    }

    read(new_socket, &num_subnets, sizeof(num_subnets));
    printf("Received number of subnets: %d\n", num_subnets);

    subnet_sizes = (int *) malloc(num_subnets * sizeof(int));

    if (!subnet_sizes) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    read(new_socket, subnet_sizes, num_subnets * sizeof(int));
    printf("Received subnet sizes: ");

    for (int i = 0; i < num_subnets; i++) {
        printf("%d ", subnet_sizes[i]);
    }

    printf("\n");

    createSubnets(ip, subnet_sizes, num_subnets, response);

    send(new_socket, response, strlen(response), 0);
    free(subnet_sizes);
    close(new_socket);
    close(server_fd);

    return 0;
}