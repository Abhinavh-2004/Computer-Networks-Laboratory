#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>
#include <string.h>
#include <netinet/in.h>  // Include this for sockaddr_in

#define PORT 8080
#define LOCALHOST "127.0.0.1"
#define BUFFER_SIZE 1024
#define BACKLOGS 5

int createSocket(int type) {
    int serverSocket = socket(AF_INET, type, 0);
    if (serverSocket < 0) {
        perror("Error creating the socket");
        exit(EXIT_FAILURE);
    }
    return serverSocket;
}

struct sockaddr_in setUp(int family, int port, const char *localhost) {
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = family;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr(localhost);
    return serverAddr;
}

void bindSocket(int sockFd, struct sockaddr *serverAddr, socklen_t addrLen) {
    if (bind(sockFd, serverAddr, addrLen) < 0) {
        perror("Error binding the socket");
        exit(EXIT_FAILURE);
    }
}

void startServer(int sockfd, int backlogs) {
    if (listen(sockfd, backlogs) < 0) {
        perror("Error starting the server");
        exit(EXIT_FAILURE);
    }
    printf("Server started at port : %d\n", PORT);
}

int acceptConnection(int sockfd, struct sockaddr *clientAddr, socklen_t *addrLen) {
    int clientSocket = accept(sockfd, clientAddr, addrLen);
    if (clientSocket < 0) {
        perror("Error accepting connection");
        exit(EXIT_FAILURE);
    }
    return clientSocket;
}

size_t len(const char *str) {
    return strlen(str);
}

void append(char *buffer, const char *time) {
    size_t bufferLength = len(buffer);
    size_t timeLength = len(time);
    memcpy(buffer + bufferLength, time, timeLength + 1);  // +1 to include null terminator
}

int chat(int sockFd) {
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));  // Clear buffer

    struct tm* ptr;
    time_t t = time(NULL);
    ptr = localtime(&t);

    printf("Enter message to client: ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        perror("Error reading input");
        return 0;
    }
    write(sockFd, buffer, strlen(buffer));

    if (strcmp(buffer, "END\n") == 0) {
        printf("End of Chat\n");
        return 0;
    }

    ssize_t bytesRead = read(sockFd, buffer, sizeof(buffer) - 1);
    if (bytesRead < 0) {
        perror("Error reading from client");
        return 0;
    }

    buffer[bytesRead] = '\0';  // Ensure null-termination
    char timeString[100];
    strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", ptr);
    append(buffer, timeString);

    printf("Message from client: %s\n", buffer);

    if (strcmp(buffer, "END\n") == 0) {
        printf("End of Chat\n");
        return 0;
    }
    return 1;
}

int main() {
    struct sockaddr_in server_addr, client_addr;
    socklen_t clientLength = sizeof(client_addr);
    int serverSocket = createSocket(SOCK_STREAM);

    server_addr = setUp(AF_INET, PORT, LOCALHOST);
    bindSocket(serverSocket, (struct sockaddr *)&server_addr, sizeof(server_addr));
    startServer(serverSocket, BACKLOGS);

    while (1) {
        int clientSocket = acceptConnection(serverSocket, (struct sockaddr *)&client_addr, &clientLength);

        while (chat(clientSocket));

        close(clientSocket);
    }

    close(serverSocket);
    return 0;
}
