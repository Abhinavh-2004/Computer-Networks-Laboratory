
#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<string.h>

#define PORT 8000
#define LocalHost "127.0.0.1"
#define BUFFER_SIZE 1024
#define BACKLOGS 5

int createSocket(int Stream){
    int serverSocket=socket(AF_INET,Stream,0);
    if(serverSocket<0){
        perror("Error! in creating the socket!(from create socket function)");
        exit(EXIT_FAILURE);
    }
    return serverSocket;
}

struct sockaddr_in setUp(int family,int port,char *localhost){
    struct sockaddr_in serverAddr;
    serverAddr.sin_family=family;
    serverAddr.sin_port=htons(port);
    serverAddr.sin_addr.s_addr=inet_addr(localhost);
    return serverAddr;
}

int bindSocket(int sockFd,struct sockaddr *serverAddr,socklen_t bufferSize){
    int returnValue=bind(sockFd,serverAddr,bufferSize);
7    if(returnValue<0){
        perror("Error! in binding the socket!( from bindSocket function)");
        exit(EXIT_FAILURE);
        
    }
    return returnValue;
}


int chat(int sockFd){
    char buffer[BUFFER_SIZE];
    struct sockaddr_in clientAddr;
    socklen_t clientLength = sizeof(clientAddr);

    
    ssize_t bytesReceived = recvfrom(sockFd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&clientAddr, &clientLength);
    if (bytesReceived < 0) {
        perror("Error receiving message from client!");
        return 0;
    }
    buffer[bytesReceived] = '\0'; 
    printf("Message from client: %s\n", buffer);

    
    if (strcmp(buffer, "Over\n") == 0) {
        printf("End of chat\n");
        return 0;
    }

    
    memset(buffer, 0, sizeof(buffer));

    
    printf("Enter a message to the client: ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        perror("Error reading input from chat function!");
        return 0;
    }
    
    ssize_t bytesSent = sendto(sockFd, buffer, strlen(buffer), 0, (struct sockaddr *)&clientAddr, clientLength);
    if (bytesSent < 0) {
        perror("Error sending message to the client!");
        return 0;
    }

    // Check if the sent message is "Over"
    if (strcmp(buffer, "Over\n") == 0) {
        printf("End of chat\n");
        return 0;
    }

    return 1;
}

int main(){
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    socklen_t clientLength;
    int serverSocket=createSocket(SOCK_DGRAM);

    server_addr=setUp(AF_INET,PORT,LocalHost);
    bindSocket(serverSocket,(struct sockaddr *)&server_addr,sizeof(struct sockaddr_in));
    

    while(1){
        

        do{
            continue;
        }while(chat(serverSocket));

        
        break;

    }   

    close(serverSocket);

}
