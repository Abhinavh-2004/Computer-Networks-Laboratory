#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define TOTAL_FRAMES 6

void receiveFrame(int sockfd, struct sockaddr_in *client_addr) {
    char buffer[1024];
    socklen_t addr_len = sizeof(*client_addr);

    while (1) {
        int n = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)client_addr, &addr_len);
        if (n > 0) {
            buffer[n] = '\0';
            printf("Received packet: %s\n", buffer);
            
            // Simulate receiving frame in order (Go-Back-N)
            int frameNum = atoi(buffer);
            if (frameNum <= TOTAL_FRAMES) {
                printf("Acknowledging frame %d\n", frameNum);
                sendto(sockfd, buffer, n, 0, (struct sockaddr *)client_addr, addr_len);
            }
        }
    }
}

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));

    // Assign IP and PORT
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket
    if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(1);
    }

    printf("Server is listening on port %d\n", PORT);

    receiveFrame(sockfd, &client_addr);

    close(sockfd);
    return 0;
}
