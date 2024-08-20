
#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<string.h>

#define PORT 8000
#define LocalHost "127.0.0.1"
#define BUFFER_SIZE 1024

int createSocket(int Stream){
    int clientSocket = socket(AF_INET, Stream, 0);
    if(clientSocket < 0){
        perror("Error! in creating the socket!(from createSocket function)");
        exit(EXIT_FAILURE);
    }
    return clientSocket;
}

struct sockaddr_in setUp(int family, int port, char *localhost){
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = family;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr(localhost);
    return serverAddr;
}

int chat(int sockFd, struct sockaddr_in *serverAddr){
    char buffer[BUFFER_SIZE];
    socklen_t serverLength = sizeof(struct sockaddr_in);
   

    printf("Enter a message to the server: ");
    if(fgets(buffer, sizeof(buffer), stdin) == NULL){
        perror("Error in reading the input (from chat function)");
        exit(EXIT_FAILURE);
    }

    ssize_t bytesSent = sendto(sockFd, buffer, strlen(buffer), 0, (struct sockaddr *)serverAddr, serverLength);
    if(bytesSent < 0){
        perror("Error in sending the message to the server!");
        exit(EXIT_FAILURE);
    }
    if(strcmp(buffer, "Over\n") == 0){
        printf("End of chat\n");
        return 0;
    }

     ssize_t bytesReceived = recvfrom(sockFd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)serverAddr, &serverLength);
    if(bytesReceived < 0){
        perror("Error in receiving the message from the server!");
        exit(EXIT_FAILURE);
    }
    buffer[bytesReceived] = '\0';  // Null-terminate the received string
    printf("Message from server: %s\n", buffer);
    if(strcmp(buffer, "Over\n") == 0){
        printf("End of chat\n");
        return 0;
    }
    return 1;
}

int main(){
    struct sockaddr_in server_addr;
    int clientSocket = createSocket(SOCK_DGRAM);

    server_addr = setUp(AF_INET, PORT, LocalHost);

    while(1){
        if(!chat(clientSocket, &server_addr)){
            break;
        }
    }

    close(clientSocket);
    return 0;
}
