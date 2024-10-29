#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 8080
#define TOTAL_FRAMES 6
#define WINDOW_SIZE 3
#define TIMEOUT 2  

void sendFrame(int sockfd, struct sockaddr_in *server_addr, int frameNum) {
    char buffer[1024];
    sprintf(buffer, "%d", frameNum);  

    sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)server_addr, sizeof(*server_addr));
    printf("Sent frame %d\n", frameNum);
}

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    struct timeval timeout;
    fd_set readfds;

    
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));

    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    printf("Client started, sending frames to server...\n");

    int nextFrameToSend = 0;
    while (nextFrameToSend < TOTAL_FRAMES) {
        
        for (int i = nextFrameToSend; i < nextFrameToSend + WINDOW_SIZE && i < TOTAL_FRAMES; i++) {
            sendFrame(sockfd, &server_addr, i + 1);
        }

        
        timeout.tv_sec = TIMEOUT;
        timeout.tv_usec = 0;
        FD_ZERO(&readfds);
        FD_SET(sockfd, &readfds);

        // Wait for an acknowledgment with a timeout
        int ret = select(sockfd + 1, &readfds, NULL, NULL, &timeout);
        if (ret > 0 && FD_ISSET(sockfd, &readfds)) {
            char ack[1024];
            struct sockaddr_in from_addr;
            socklen_t addr_len = sizeof(from_addr);
            int n = recvfrom(sockfd, ack, sizeof(ack), 0, (struct sockaddr *)&from_addr, &addr_len);
            if (n > 0) {
                ack[n] = '\0';
                printf("Received ACK: %s\n", ack);
            }
        } else {
            printf("Timeout occurred, retransmitting...\n");
        }

        // Slide window forward
        nextFrameToSend += WINDOW_SIZE;
    }

    close(sockfd);
    return 0;
}
