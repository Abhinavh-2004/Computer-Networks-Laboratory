#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <time.h>

#define PORT 8080
#define LOCALHOST "127.0.0.1"
#define BUFFER_SIZE 1024

int createSocket(int type) {
    int clientSocket = socket(AF_INET, type, 0);
    if (clientSocket < 0) {
        perror("Error creating the socket");
        exit(EXIT_FAILURE);
    }
    return clientSocket;
}

struct sockaddr_in setUp(int family, int port, const char *localhost) {
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = family;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr(localhost);
    return serverAddr;
}

int connectSocket(int sockFd, struct sockaddr *addr, socklen_t size) {
    int returnValue = connect(sockFd, addr, size);
    if (returnValue < 0) {
        perror("Error connecting the socket");
        exit(EXIT_FAILURE);
    }
    return returnValue;
}

size_t len(const char *str) {
    return strlen(str);
}

void append(char *buffer, const char *time) {
    size_t bufferLength = len(buffer);
    size_t timeLength = len(time);
    memcpy(buffer + bufferLength, time, timeLength + 1);  // +1 to include null 
}

int chat(int sockFd) {
    char buffer[BUFFER_SIZE];
    struct tm *ptr;
    time_t t;
    t = time(NULL);
    ptr = localtime(&t);

    ssize_t bytesRead = read(sockFd, buffer, sizeof(buffer) - 1);
    if (bytesRead < 0) {
        perror("Error reading from server");
        return 0;
    }

    buffer[bytesRead] = '\0';  

    char timeString[100];
    strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", ptr);
    append(buffer, timeString);

    if (strcmp(buffer, "END\n") == 0) {
        printf("End of Chat\n");
        return 0;
    }

    printf("Message from server: %s\n", buffer);

    printf("Enter a message to send to the server: ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        perror("Error reading input");
        return 0;
    }

    write(sockFd, buffer, strlen(buffer));

    if (strcmp(buffer, "END\n") == 0) {
        printf("End of Chat\n");
        return 0;
    }

    return 1;
}

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;
    clientSocket = createSocket(SOCK_STREAM);

    serverAddr = setUp(AF_INET, PORT, LOCALHOST);

    connectSocket(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

    while (chat(clientSocket));  

    close(clientSocket);
    return 0;
}
