#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

// PORT number
#define PORT 4445
#define BUFFER_SIZE 1024

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE];
    ssize_t bytesRead;

    // Create socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        perror("socket() failed");
        exit(EXIT_FAILURE);
    }

    // Setup server address structure
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("connect() failed");
        close(clientSocket);
        exit(EXIT_FAILURE);
    }

    printf("Connected to server.\n");

    while (1) {
        // Get input from user
        printf("Enter message: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline character

        // Send message to the server
        send(clientSocket, buffer, strlen(buffer), 0);

        // Receive response from the server
        bytesRead = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
        if (bytesRead > 0) {
            buffer[bytesRead] = '\0'; // Null-terminate the string
            printf("Server: %s\n", buffer);
        } else if (bytesRead == 0) {
            printf("Server closed the connection.\n");
            break;
        } else {
            perror("recv() failed");
            break;
        }
    }

    close(clientSocket);
    return 0;
}
