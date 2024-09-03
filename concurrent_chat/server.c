#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

// PORT number
#define PORT 4445
#define BUFFER_SIZE 1024

// Function to handle client connections
void *handle_client(void *arg) {
    int clientSocket = *(int *)arg;
    char buffer[BUFFER_SIZE];
    ssize_t bytesRead;

    while ((bytesRead = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0)) > 0) {
        buffer[bytesRead] = '\0'; // Null-terminate the string
        printf("Received: %s\n", buffer);
        send(clientSocket, buffer, bytesRead, 0); // Echo back the message
        memset(buffer, 0, BUFFER_SIZE); // Clear the buffer
    }

    if (bytesRead < 0) {
        perror("recv() failed");
    }

    close(clientSocket);
    pthread_exit(NULL);
}

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    pthread_t threadId;

    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        perror("socket() failed");
        exit(EXIT_FAILURE);
    }

    // Setup server address structure
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    // Bind the socket
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("bind() failed");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(serverSocket, 5) < 0) {
        perror("listen() failed");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        // Accept a client connection
        clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen);
        if (clientSocket < 0) {
            perror("accept() failed");
            continue;
        }

        // Create a new thread to handle the client
        if (pthread_create(&threadId, NULL, handle_client, (void *)&clientSocket) != 0) {
            perror("pthread_create() failed");
            close(clientSocket);
        } else {
            pthread_detach(threadId); // Automatically reclaim the thread resources when it terminates
        }
    }

    close(serverSocket);
    return 0;
}
